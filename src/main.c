#include "parser.h"
#include "utf8.h"

#include <stdio.h>

int main() {
	char* in = "####### Título";
	printf("%s\n", in);

	consumewhile(&in, "#", 6);
	printf("%s\n", in);
	return 0;
}