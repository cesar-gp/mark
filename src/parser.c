#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utf8.h"

uint32_t consumewhile(char** in, char* str, uint32_t max) {
	char* strorigin = str;

	uint32_t enc;
	uint32_t strenc;
	uint8_t len;
	uint8_t strlen;
	char next[5];
	char strnext[5];
	uint32_t read = 0;
	uint32_t strread = 0;
	uint32_t repe = 0;

	for(; repe < max; *in += len, str += strlen, strread++, read++) {
		// Obtener siguiente caracter de '*in' y salir si no es válido.
		if(utf8decode(*in, &enc, &len)) break;
		memcpy(next, *in, len);
		next[len] = '\0';

		// ¿'str' consumida? Resetear.
		if(!*str) {
			str = strorigin;
			strread = 0;
			repe++;
		}

		// Obtener siguiente caracter de 'str' y salir si no es válido.
		if(utf8decode(str, &strenc, &strlen)) break;
		memcpy(strnext, str, strlen);
		strnext[strlen] = '\0';

		// ¿No coinciden los caracteres? Salir.
		if(strcmp(next, strnext)) break;
	}

	read -= strread;
	*in -= str - strorigin;

	return read;
}