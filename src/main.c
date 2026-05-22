
#include <err.h>
#include <stdio.h>

#include "evaluate.c"

//------------------------------------------------------------------------------
//
// ENTRY POINT
//
//______________________________________________________________________________

int main(void) {

	// getline() buffer
	char*   buf = NULL;
	size_t  cap = 0;
	ssize_t len = 0;

	// User prompt
	fputs("Mathulator ⭢  ", stderr);

	// Wait for input
	len = getline(&buf, &cap, stdin);

	// CONTINUE if received input

	if(len <= 0) {
		fputs("\n", stderr);
		err(1, "Failed to read line");
	}

	buf[len - 1] = 0; // Clear newline

	parse_string(buf, (size_t) len); // Enter recursive evaluation phase
}
