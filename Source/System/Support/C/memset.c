//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "string.h"

__attribute__((noinline))
void* memset(void* s, int c, size_t count) {
	for(size_t i = 0; i < count; i++)
		((uint8_t*) s)[i] = (uint8_t) c;
	return s;
}