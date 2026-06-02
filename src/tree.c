#pragma once

#include "utils.c"

#define TREE_ROOT 0

////////////////////////////////////////////////////////////////////////////////

/* A symbol will be IDENT if it represents a variable, or PROSE if it should be
   ignored before apply phase */
#define TOKENTYPES X(NONE) X(IDENT) X(PROSE)
typedef enum {
#define X(x) x,
	TOKENTYPES
#undef X
} TokenTypes;

/* Returns a null-terminated string of the string representation of a value of
   the TokenType enum */
const char* show_tokentype(TokenTypes type);

// Stores recognized variable identifiers and their definition in string
// form
ArrayN(struct Symbol*, Symbols);
ArrayN(struct Symbol, SymbolStorage);

struct Symbol {
	TokenTypes type;
	StringView key;
	Symbols    children;
};

/* Working storage of all symbols. Functions as a definition tree which holds
   the expansion of all variables. Is used for Step 1 - 3 of recursive
   evaluation. */
extern struct Symbol symbols_root;
extern SymbolStorage symbols_store;

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

struct Symbol symbols_root  = {0};
SymbolStorage symbols_store = {0};

const char* show_tokentype(TokenTypes type) {
	switch(type) {
		// clang-format off
#define X(x) case x: return #x;

TOKENTYPES

#undef X
		// clang-format on
	}
}

#endif
