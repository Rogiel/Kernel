//
// Created by Rogiel Sulzbach on 9/16/17.
//

#pragma once

#include <stdint.h>

typedef unsigned int size_t;

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dst, const void* src, size_t size);
void* memset(void * s,int c,size_t count);

void _Unwind_Resume(void* obect);

void* memmove(void* dst, const void* src, size_t len);
void strcpy();
void strncpy();
void strcat();
void strncat();
void memcmp();
void strcmp();
void strncmp();
void strcoll();
void strxfrm();
void memchr();
void strchr();
void strcspn();
void strpbrk();
void strrchr();
void strspn();
void strstr();
void strtok();
void strerror();
size_t strlen(const char* str);

#ifdef __cplusplus
}
#endif