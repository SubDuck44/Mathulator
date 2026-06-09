
#include <err.h>

#include "apply.c"
#include "evaluate.c"

int main(void) {
	while(true) {
		parse_string(
			ask_for_definition(&symbols_root), &symbols_root
		); // Enter recursive evaluation phase

		fprintf(stderr, "Finished parsing, dumping storage...\n");
		PRINT_RESULTS(&, symbols_store);

		make_queue(&symbols_root);

		fprintf(
			stderr, "Finished evaluating definition order, dumping result...\n"
		);
		PRINT_RESULTS(, queue);

		fprintf(stderr, "Finished expression, dumping final results...\n");
		pretty_print(queue);
	}
}
