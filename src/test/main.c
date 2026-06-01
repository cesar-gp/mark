#include <string.h>
#include <stdio.h>
#include "assert.h"
#include "testutf8.h"

int main() {
	putc('\n', stdout);
	
	test_utf8len();
	test_utf8chars();
	test_utf8decode();
	
	putc('\n', stdout);
	return 0;
}