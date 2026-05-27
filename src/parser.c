#include <stdio.h>

int parserep(char** in, char** buf, char c, bool write) {
	char* origin = *in;

	while(**in == c) {
		if(write) *(*buf)++ = **in;
		(*in)++;
	}

	return *in - origin;
}