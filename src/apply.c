#pragma once

#include "tree.c"

#include <string.h>

/* Sweeps through the symbol tree & storage to find all related symbols and
   outputs the resulting typst-compatible expression to stdout */
void print_results(void);

#if __INCLUDE_LEVEL__ == 0 /////////////////////////////////////////////////////

void print_results(void) {
#define BUF_LEN 1024

	for(size_t i = 0; i < symbols_store.len; i++) {
		struct Symbol* sym = &symbols_store.ptr[i];

		// Write readout header
		fprintf(
			stderr, "SYMBOL:\n\tKey: %.*s\n", (int) sym->key.len, sym->key.str
		);
		fprintf(stderr, "\tType: %s\n", show_tokentype(sym->type));
		fprintf(stderr, "\tChildren: ");

		// Write children to readout
		for(size_t j = 0; j < sym->children.len; j++) {
			StringView child_key = sym->children.ptr[j]->key;

			fprintf(stderr, "%.*s, ", (int) child_key.len, child_key.str);
		}

		// Finish writing readout
		fprintf(stderr, "\n");
	}

#undef BUF_LEN
}

#endif
