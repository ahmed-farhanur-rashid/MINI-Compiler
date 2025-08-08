#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Auto-cleanup system
static void **__cleanup_ptrs = NULL;
static int __cleanup_count = 0;
static int __cleanup_capacity = 0;

void __register_ptr(void *ptr) {
    if (__cleanup_count >= __cleanup_capacity) {
        __cleanup_capacity = __cleanup_capacity ? __cleanup_capacity * 2 : 16;
        __cleanup_ptrs = realloc(__cleanup_ptrs, __cleanup_capacity * sizeof(void*));
    }
    __cleanup_ptrs[__cleanup_count++] = ptr;
}

void __cleanup_all() {
    for (int i = 0; i < __cleanup_count; i++) {
        free(__cleanup_ptrs[i]);
    }
    free(__cleanup_ptrs);
}

int main() {
    double x;
    x = 42;
    const char* simple;
    simple = ({
    char* __strtmp0 = malloc(64);
    sprintf(__strtmp0, "%.15g", (double)(x));
    __strtmp0;
});
    { printf("%s", simple); }
    __cleanup_all();
    return 0;
}
