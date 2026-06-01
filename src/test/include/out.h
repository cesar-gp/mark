#pragma once

#include <stdio.h>
#include <stdlib.h>

inline void out_ok() {
	printf("ok.\n");
}

inline void out_errstart() {
	printf("error.\n   ");
}

inline void out_errend() {
	printf("\n\n");
	exit(1);
}

inline void out_start(char* name) {
	printf(" - Probando '%s' ... ", name);
}