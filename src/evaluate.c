#pragma once

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.c"

/* Asks for user input returns the resulting string. To be passed to
   parse_string(), which then initiates the recursive evaluation. Optionally
   pass a pointer to the symbol to be defined, if its "NULL", the prompt will
   simply be empty.
   -> Step 1 of recursive evaluation */
StringView ask_for_definition(const struct Symbol* target);

/* Splits an input string from the user into tokens and recursively prompts to
   define them -> Step 2 of rescursive evaluation */
void parse_string(StringView string, struct Symbol* parent);

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

/* List of identifiers that should not be interpreted as variable names */
static char* id_blacklist[] = {"sqrt", "pow",  "log",  "sin",  "cos",  "tan",
                               "cot",  "asin", "acos", "atan", "acot", "pi"};

/* Takes a string and its length and returns PROSE if the string is part of the
   symbol blacklist, else returns IDENT */
static TokenTypes check_if_blacklisted(StringView token) {
	// Skip target identifier if it's blacklistedi
	size_t num_blacklisted_ids = sizeof(id_blacklist) / sizeof(id_blacklist[0]);

	for(size_t iter = 0; iter < num_blacklisted_ids; iter++) {

		// Avoid matching shorter substrings of keywords
		if(strlen(id_blacklist[iter]) != token.len) continue;

		if(strncmp(id_blacklist[iter], token.str, token.len) == 0) {

			fprintf(
				stderr, "Ignored blacklisted identifier %.*s\n",
				(int) token.len, token.str
			);

			return PROSE;
		}
	}

	return IDENT;
}

/* Filters a newly found token into the symbol tree, takes the "new_symbol"
   itself and a reference to its "parent"
   -> Step 3 of recursive evaluation, loops back to Step 1*/
static void store_symbol(struct Symbol new_symbol, struct Symbol* parent) {
#define PERSIST_SYM &ArrayLast(symbols_store)

	switch(new_symbol.type) {
	case IDENT:
		// Abort if symbol is duplicate with another id
		ArrayLoop(
			symbols_store,
			if(new_symbol.key_len == it->key_len &&                       //
		       strncmp(new_symbol.key, it->key, new_symbol.key_len) == 0) //
			return;
		);

		// Make symbol persistent
		ArrayAdd(symbols_store, new_symbol);

		parse_string(ask_for_definition(&new_symbol), PERSIST_SYM);

		// Append persistent symbol to tree
		ArrayAdd(parent->children, PERSIST_SYM);
		break;

	case PROSE:

		// Make symbol persistent
		ArrayAdd(symbols_store, new_symbol);

		// Append persistent symbol to tree
		ArrayAdd(parent->children, PERSIST_SYM);
		break;

	default:
		return;
	}
}

StringView ask_for_definition(const struct Symbol* target) {
	if(target == NULL) {
		fputs("Mathulator\n➔  ", stderr);
	} else {
		fprintf(stderr, "Define %.*s\n➔  ", (int) target->key_len, target->key);
	}

	UserInput input = {0};
	get_user_input(&input);

	return (StringView) {strndup(input.ptr, input.len), input.len};
}

void parse_string(StringView string, struct Symbol* parent) {

#define MATCH_PTR &string.str[match_start]
#define MATCH_LEN match_end - match_start + 1

// Integrates newly found symbols into the symbol tree
#define STORE_SYMBOL(ident_or_prose)                                           \
	do {                                                                       \
		/* Pack new symbol */                                                  \
		struct Symbol new_sym = {                                              \
			.type    = (ident_or_prose),                                       \
			.key     = MATCH_PTR,                                              \
			.key_len = MATCH_LEN,                                              \
		};                                                                     \
                                                                               \
		store_symbol(new_sym, parent);                                         \
	} while(0)

// Evaluates to true if 'c' is a valid char for an identifier
#define IS_IDENT(c)                                                            \
	(((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || (c) == '_')

	size_t match_start = 0;
	size_t match_end   = 0;

	TokenTypes is_matching = NONE;

	// Scan for tokens
	for(size_t iter = 0; iter < string.len; iter++) {
		char c = string.str[iter];

		if(IS_IDENT(c)) {
			fprintf(stderr, "Found identifier char %c\n", c);
			switch(is_matching) {
			case NONE:
				// Start matching identifier
				match_start = iter;
				match_end   = iter;
				is_matching = IDENT;
				break;

			case IDENT:
				// Continue matching identifier
				match_end = iter;
				break;

			case PROSE:
				// Reached end of prose match, store value
				fprintf(
					stderr, "Terminated %.*s with %c\n", (int) (MATCH_LEN),
					MATCH_PTR, c
				);
				STORE_SYMBOL(PROSE);

				match_start = iter;
				match_end   = iter;
				is_matching = IDENT;
				break;
			}
		} else {
			fprintf(stderr, "Found prose char %c\n", c);
			switch(is_matching) {
			case NONE:
				// Start matching prose
				match_start = iter;
				match_end   = iter;
				is_matching = PROSE;
				break;

			case IDENT:
				// Reached end of identifier match, store value
				fprintf(
					stderr, "Terminated %.*s with %c\n", (int) (MATCH_LEN),
					MATCH_PTR, c
				);
				STORE_SYMBOL(check_if_blacklisted((StringView) {MATCH_PTR,
				                                                MATCH_LEN}));

				match_start = iter;
				match_end   = iter;
				is_matching = PROSE;
				break;

			case PROSE:
				// Continue matching prose
				match_end = iter;
				break;
			}
		}
	}
}

#endif
