#pragma once

#include <stdint.h>

typedef enum {
	REPEATER			= 1,
	DELIMITER			= 2
} consumer_t;

uint32_t consumestr(char** in, char* str, uint32_t max, consumer_t consumer);