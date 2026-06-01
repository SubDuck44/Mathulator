
#include <err.h>

#include "evaluate.c"

int main(void) {
	while(true) {
		parse_string(
			ask_for_definition(&symbols_root), &symbols_root
		); // Enter recursive evaluation phase
	}
}
