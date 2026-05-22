
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project components
#include "utils.c"

// Stores recognized variable identifiers and their definition in string form
typedef struct {
	char*  key;
	size_t key_len;
	char*  val;
	size_t val_len;
} Symbol;

Array(Symbol);

/* Working storage of all symbols. Functions as a definition tree which holds
   the expansion of all variables. Is used for Step 1 - 3 of recursive
   evaluation. */
Symbols symbols = {0};

/* Splits an input string from the user into tokens and recursively prompts to
   define them -> Step 1 of rescursive evaluation */
void parse_string(const char* target, size_t len);

/* Packs a symbol identifier into a "Symbol" and calls ask_for_definition() to
   fill out the definition. -> Step 2 of recursive evaluation */
void store_symbol(const char* target, size_t len);

/* Asks the user to define a certain symbol; stores response in value of symbol,
   then calls parse_string() to evaluate the given input string for further
   valid symbol identifiers. If none are found, the recursion breaks and the
   tree of symbols is deemed complete. -> Step 3 of recursive evaluation */
void ask_for_definition(Symbol* target);

/* List of identifiers that should not be interpreted as variable names */
char* id_blacklist[] = {"sqrt", "pow",  "log",  "sin",  "cos",  "tan",
                        "cot",  "asin", "acos", "atan", "acot", "pi"};
//------------------------------------------------------------------------------
//
// IMPLEMENTATION --------------------------------------------------------------
//
//______________________________________________________________________________

void parse_string(const char* target, size_t len) {
	size_t match_start = 0;
	size_t match_end   = 0;

	bool in_match = false;

	// Scan for valid variable identifiers in target string
	for(size_t iter = 0; iter <= len; iter++) {

		char c = target[iter];

		if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {

			// Set start of match here if not already in match
			if(!in_match) {
				match_start = iter;
				in_match    = true;
			}

			// Extend the match if already in match
			match_end = iter;

		} else if(in_match) {

			fprintf(
				stderr, "Terminated match %.*s with %c\n",
				(int) (match_end - match_start + 1), &target[match_start], c
			);

			/* End and store match if previously in match and current
			   character not matchable */
			store_symbol(&target[match_start], match_end - match_start + 1);

			in_match = false;
		}
	}
}

void store_symbol(const char* target, size_t target_len) {

	// Skip target identifier if it's blacklisted
	size_t num_blacklisted_ids = sizeof(id_blacklist) / sizeof(id_blacklist[0]);

	for(size_t iter = 0; iter < num_blacklisted_ids; iter++) {

		// Avoid matching shorter substrings of keywords
		if(strlen(id_blacklist[iter]) != target_len) continue;

		if(!strncmp(id_blacklist[iter], target, target_len)) {

			fprintf(
				stderr, "Ignored blacklisted identifier %.*s\n",
				(int) target_len, target
			);

			return;
		}
	}

	// Skip target identifier if a duplicate is already present in symbol tree
	for(size_t iter = 0; iter < symbols.len; iter++) {

		if(!strncmp(symbols.ptr[iter].key, target, target_len)) {

			fprintf(
				stderr, "Ignored string %.*s due to match with duplicate\n",
				(int) target_len, target
			);

			return;
		}
	}

	// Found valid new identifier
	Symbol new_symbol = {strndup(target, target_len), target_len, NULL, 0};

	// Fill in missing definition
	ask_for_definition(&new_symbol); // RECURSION here

	// Push finished symbol
	ArrayAdd(symbols, new_symbol);
}

void ask_for_definition(Symbol* symbol) {

	char*   buf = NULL; // String buffer, allocated by getline
	size_t  cap = 0;
	ssize_t len = 0;

	// Prompt user
	fprintf(stderr, "Define %.*s ⭢  ", (int) symbol->key_len, symbol->key);
	len = getline(&buf, &cap, stdin);

	// CONTINUE if received input

	if(len <= 0) {
		fputs("\n", stderr); // Hack to avoid jumbling the error message
		err(1, "Failed to read line");

		return;
	}

	// Store user definition
	symbol->val     = buf;
	symbol->val_len = (size_t) len;

	// Scan for further valid symbol identifiers in definition
	parse_string(symbol->val, symbol->val_len); // RECURSION here
}

//------------------------------------------------------------------------------
//
// ENTRY POINT
//
//______________________________________________________________________________

int main(void) {

	// getline() buffer
	char*   buf = NULL;
	size_t  cap = 0;
	ssize_t len = 0;

	// User prompt
	fputs("Mathulator ⭢  ", stderr);

	// Wait for input
	len = getline(&buf, &cap, stdin);

	// CONTINUE if received input

	if(len <= 0) {
		fputs("\n", stderr);
		err(1, "Failed to read line");
	}

	buf[len - 1] = 0; // Clear newline

	parse_string(buf, (size_t) len); // Enter recursive evaluation phase
}
