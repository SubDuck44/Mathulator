#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.c"

typedef struct {
	char*  key;
	size_t key_len;
	char*  val;
	size_t val_len;
} Node;
DynArr(Node);

void ask_value(Node* symbol);
void store_symbol(const char* target, size_t len);
void parse_string(const char* target, ssize_t len);

Nodes symbols = {0};

char* keywords[] = {"sqrt", "pow",  "log",  "sin",  "cos",  "tan",
                    "cot",  "asin", "acos", "atan", "acot", "pi"};

void ask_value(Node* symbol) {
	char*   buf = NULL;
	size_t  cap = 0;
	ssize_t len = 0;

	fprintf(stderr, "Define %.*s ⭢  ", (int) symbol->key_len, symbol->key);
	len = getline(&buf, &cap, stdin);
	if(len <= 0) {
		fputs("\n", stderr);
		err(1, "Failed to read line");
		return;
	}
	symbol->val     = buf;
	symbol->val_len = len;
	parse_string(symbol->val, symbol->val_len);
}

void store_symbol(const char* target, size_t len) {
	// Ignore if keyword
	for(size_t iter = 0; iter < sizeof(keywords) / sizeof(char*); iter++) {
		if(strlen(keywords[iter]) != len) continue;
		if(!strncmp(keywords[iter], target, len)) {
			fprintf(
				stderr, "Ignored string %.*s due to match with keyword\n",
				(int) len, target
			);
			return;
		}
	}

	// Ignore if duplicate
	for(size_t iter = 0; iter < symbols.len; iter++) {
		if(!strncmp(symbols.arr[iter].key, target, len)) {
			fprintf(
				stderr, "Ignored string %.*s due to match with duplicate\n",
				(int) len, target
			);
			return;
		}
	}

	// Add new symbol to table
	Node new_symbol = {strndup(target, len), len, NULL, 0};
	ask_value(&new_symbol);
	DynArrPush(&symbols, new_symbol);
	fprintf(
		stderr, "Added symbol %.*s at index %ld\n", (int) len, target,
		symbols.len - 1
	);
}

void parse_string(const char* target, ssize_t len) {
	size_t match_start = 0;
	size_t match_end   = 0;
	bool   in_match    = false;

	// Search for matchable characters
	for(ssize_t iter = 0; iter <= len; iter++) {
		char c = target[iter];
		if((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95) {
			fprintf(stderr, "Found matchable char %c at %lu\n", c, iter);
			// Extend the match if character is matchable
			if(!in_match) {
				match_start = iter;
				in_match    = true;
			}
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

int main(void) {
	char*  buf = NULL;
	size_t cap = 0;

	fputs("Mathulator ⭢  ", stderr);
	ssize_t len = getline(&buf, &cap, stdin);
	if(len <= 0) {
		fputs("\n", stderr);
		err(1, "Failed to read line");
	}
	buf[len - 1] = 0;
	parse_string(buf, len);
}
