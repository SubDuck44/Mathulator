#pragma once

#include "utils.c"

#define TREE_ROOT 0

////////////////////////////////////////////////////////////////////////////////

/* A symbol will be IDENT if it represents a variable, or PROSE if it should be
   ignored before apply phase */
typedef enum {
	NONE,
	IDENT,
	PROSE,
} TokenTypes;

// Stores recognized variable identifiers and their definition in string form
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

#endif
