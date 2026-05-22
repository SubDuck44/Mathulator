#pragma once

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.c"

/* Splits an input string from the user into tokens and recursively prompts to
   define them -> Step 1 of rescursive evaluation */
void parse_string(const char* target, size_t target_len, const size_t parent);

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

/* List of identifiers that should not be interpreted as variable names */
static char* id_blacklist[] = {"sqrt", "pow",  "log",  "sin",  "cos",  "tan",
                               "cot",  "asin", "acos", "atan", "acot", "pi"};

/* Takes a string and its length and returns PROSE if the string is part of the
   symbol blacklist, else returns IDENT */
TokenTypes check_for_blacklisted(const char* target, size_t target_len) {
	// Skip target identifier if it's blacklistedi
	size_t num_blacklisted_ids = sizeof(id_blacklist) / sizeof(id_blacklist[0]);

	for(size_t iter = 0; iter < num_blacklisted_ids; iter++) {

		// Avoid matching shorter substrings of keywords
		if(strlen(id_blacklist[iter]) != target_len) continue;

		if(strncmp(id_blacklist[iter], target, target_len) == 0) {

			fprintf(
				stderr, "Ignored blacklisted identifier %.*s\n",
				(int) target_len, target
			);

			return PROSE;
		}
	}

	return IDENT;
}

void parse_string(const char* target, size_t target_len, const size_t parent) {
	assert(symbols.ptr[0].key == NULL && "Index 0 is not root");
	assert(parent == symbols.len - 1 && "Parent is not last element in tree");
	assert(parent <= 0 && parent < symbols.len && "Parent index out of range");

#define STORE_MATCHED(prose_or_ident)                                          \
	do {                                                                       \
		size_t match_len = match_end - match_start + 1;                        \
                                                                               \
		Symbol matched_symbol = {                                              \
			.key     = strndup(&target[iter], match_len),                      \
			.key_len = match_len,                                              \
		};                                                                     \
		matched_symbol.type =                                                  \
			check_for_blacklisted(matched_symbol.key, matched_symbol.key_len); \
                                                                               \
		if(!matched_symbol.key) err(1, "strndup failure, aborting...");        \
                                                                               \
		ArrayAdd(symbols, matched_symbol);                                     \
	} while(0)

// Evaluates to true if 'c' is a valid char for an identifier
#define IS_IDENT(c)                                                            \
	(((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || (c) == '_')

	size_t match_start = 0;
	size_t match_end   = 0;

	TokenTypes is_matching = NONE;

	size_t num_matches = 0;

	// Scan for tokens
	for(size_t iter = 0; iter < target_len; iter++) {
		char c = target[iter];

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
					stderr, "Terminated %.*s with %c\n",
					(int) (match_end - match_start + 1), &target[match_start], c
				);
				STORE_MATCHED(PROSE);
				num_matches++;

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
					stderr, "Terminated %.*s with %c\n",
					(int) (match_end - match_start + 1), &target[match_start], c
				);
				STORE_MATCHED(IDENT);
				num_matches++;

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

	// Assign all matches as children of designated parent node in tree
	symbols.ptr[parent].children     = symbols.len - num_matches;
	symbols.ptr[parent].num_children = num_matches;

	assert(
		symbols.ptr[parent].children + symbols.ptr[parent].num_children ==
			symbols.len &&
		"Assigned incorrect children range to parent"
	);

	// Repeat procedure for all matched tokens
	UserInput input = {0};

	for(size_t iter = parent + 1; iter < symbols.len; iter++) {
		if(symbols.ptr[iter].type != IDENT) continue;

		get_user_input(&input);
		fprintf(stderr, "Define %.*s\n➔  ", (int) input.len, input.ptr);
		parse_string(input.ptr, input.len, iter);
	}

	ArrayFree(input);
}

#endif
