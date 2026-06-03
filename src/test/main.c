#include <string.h>
#include <stdio.h>
#include "assert.h"
#include "testutf8.h"
#include "testparser.h"

int main() {
	putc('\n', stdout);
	
	test_utf8charlen();
	test_utf8chars();
	test_utf8decode();

	test_consumestr();
	
	putc('\n', stdout);
	return 0;
}