#pragma once

#include <stdlib.h>

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
typedef struct {
	TokenTypes type;
	char*      key;
	size_t     key_len;
	size_t     children;
	size_t     num_children;
} Symbol;

Array(Symbol);

/* Working storage of all symbols. Functions as a definition tree which holds
   the expansion of all variables. Is used for Step 1 - 3 of recursive
   evaluation. */
extern Symbols symbols;

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

Symbols symbols = {0};

#endif
