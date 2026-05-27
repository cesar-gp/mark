#include "assert.h"
#include "testformat.h"
#include "parser.h"

void parserep_test() {
	test_start("parserrep");
	// Initialize input.
	char* in = "abbcccddddeeeeedccbbbaaaa";

	// Initialize buffer.
	char array[1024];
	char* buf = (char*) array;

	// Parse input.
	while(*in) {
		*buf++ = *in;
		parserep(&in, &buf, *in, false);
	}
	*buf = '\0';

	assert_equals(array, "abcdedcba");

	test_ok();
}