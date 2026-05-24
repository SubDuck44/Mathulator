
#include <err.h>

#include "evaluate.c"

int main(void) {
	struct Symbol tree_root = {0};

	while(true) {
		parse_string(
			ask_for_definition(&tree_root), &tree_root
		); // Enter recursive evaluation phase
	}
}
