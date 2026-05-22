
#include <err.h>
#include <stdio.h>

#include "evaluate.c"

int main(void) {
	// Init symbol tree
	Symbol root = {0};
	ArrayAdd(symbols, root);

	fputs("Mathulator\n➔  ", stderr);
	UserInput input = {0};
	get_user_input(&input);

	// CONTINUE if received input

	input.ptr[input.len - 1] = 0; // Clear newline

	parse_string(
		input.ptr, (size_t) input.len, TREE_ROOT
	); // Enter recursive evaluation phase
}
