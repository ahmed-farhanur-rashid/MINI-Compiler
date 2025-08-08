#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Auto-cleanup system
static const void **__cleanup_ptrs = NULL;
static int __cleanup_count = 0;
static int __cleanup_capacity = 0;

void __register_ptr(const void *ptr) {
    if (__cleanup_count >= __cleanup_capacity) {
        __cleanup_capacity = __cleanup_capacity ? __cleanup_capacity * 2 : 16;
        __cleanup_ptrs = realloc(__cleanup_ptrs, __cleanup_capacity * sizeof(const void*));
    }
    __cleanup_ptrs[__cleanup_count++] = ptr;
}

void __cleanup_all() {
    for (int i = 0; i < __cleanup_count; i++) {
        free((void*)__cleanup_ptrs[i]);
    }
    free(__cleanup_ptrs);
}

int main() {
    const char* a = "Hello";
    const char* b = " World";
    char* __strtmp3000 = malloc(strlen(a) + strlen(b) + 1); __register_ptr(__strtmp3000);
    __strtmp3000[0] = '\0';
    strcat(__strtmp3000, a);
    strcat(__strtmp3000, b);
    const char* result = __strtmp3000;
    { printf("%s", result); }
    __cleanup_all();
    return 0;
}
