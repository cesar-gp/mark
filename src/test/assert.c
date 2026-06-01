#include "out.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void assert_strequals(char* a, char* b) {
	if(!strcmp(a, b)) return;

	out_errstart();
	printf("Se esperaba '%s' pero se ha obtenido '%s'.", b, a);
	out_errend();
}

void assert_uintequals(uint64_t a, uint64_t b) {
	if(a == b) return;

	out_errstart();
	printf("Se esperaba '%lu' pero se ha obtenido '%lu'", b, a);
	out_errend();
}