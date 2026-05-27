#pragma once

#include <stdio.h>
#include <stdlib.h>

inline void test_ok() {
	printf("ok.\n");
}

inline void test_errstart() {
	printf("error.\n   ");
}

inline void test_errend() {
	printf("\n\n");
	exit(1);
}

inline void test_start(char* name) {
	printf(" - Probando '%s' ... ", name);
}