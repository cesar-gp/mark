#pragma once

#include <stdint.h>

uint8_t utf8len(const char* in);
uint32_t utf8chars(const char* in);
uint8_t utf8decode(const char* in, uint32_t* out, uint8_t* len);