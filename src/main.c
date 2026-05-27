#include "parser.h"
#include <stdio.h>

int main() {
	// Initialize input.
	char* in = "1 2  3    4    5     6         7  ";

	// Initialize buffer.
	char array[1024];
	char* buf = (char*) array;

	// Parse input.
	while(*in) {
		if(*in != ' ') *buf++ = *in++;
		else parserep(&in, &buf, ' ', false);
	}

	*buf = '\0';

	// Output result.
	printf(" -> '%s'\n", array);

	// Operation succeeded.
	return 0;
}