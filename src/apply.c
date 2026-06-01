#pragma once

#include "tree.c"

/* Sweeps through the symbol tree & storage to find all related symbols and
   outputs the resulting typst-compatible expression to stdout */
void print_results(void);

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

void print_results(void) {
	for(size_t iter = 0; iter < symbols_store.len; iter++) {
		struct Symbol* sym = &symbols_store.ptr[iter];
		fprintf(stdout, "SYMBOL:\n\tKey: %.*s\n\tChildren: %.*s\n");
	}
}

#endif
