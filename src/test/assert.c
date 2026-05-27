#include "testformat.h"
#include <stdio.h>
#include <string.h>

void assert_equals(char* a, char* b) {
	if(!strcmp(a, b)) return;

	test_errstart();
	printf("Se esperaba '%s' pero se ha obtenido '%s'.", b, a);
	test_errend();
}