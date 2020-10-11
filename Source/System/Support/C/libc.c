//
// Created by Rogiel Sulzbach on 9/16/17.
//

#include "string.h"

#include <stdarg.h>

void __stderrp() {

}

void vfprintf() {

}

void fprintf() {

}

void abort() {

}

void strcmp() {

}

void memcmp() {

}

void _Unwind_Resume(void* obect) {

}

void _DefaultRuneLocale() {

}

void __maskrune() {

}

void* memmove(void* dst, const void* src, size_t len) {
	return memcpy(dst, src, len);
}

size_t strlen(const char* str) {
	for(int i = 0;; i++) {
		if(str[i] == 0x00) {
			return i;
		}
	}
}



//
//void __gxx_personality_v0() {
//
//}