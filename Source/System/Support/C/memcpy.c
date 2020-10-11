//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "string.h"

__attribute__((noinline))
void* memcpy(void* dst, const void* src, size_t size) {
	for(size_t i = 0; i < size; i++)
		((uint8_t*) dst)[i] = ((const uint8_t*) src)[i];
	return dst;
}