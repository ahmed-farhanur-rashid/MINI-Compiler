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
    const char* simple = "Hello";
    { char* __temp = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    {
        size_t old_len = strlen(__strtmp0);
        char* expr_val = strdup(simple);
        size_t expr_len = strlen(expr_val);
        __strtmp0 = realloc(__strtmp0, old_len + expr_len + 1);
        strcat(__strtmp0, expr_val);
        free(expr_val);
    }
    {
        size_t old_len = strlen(__strtmp0);
        char* expr_val = strdup(" World");
        size_t expr_len = strlen(expr_val);
        __strtmp0 = realloc(__strtmp0, old_len + expr_len + 1);
        strcat(__strtmp0, expr_val);
        free(expr_val);
    }
    __register_ptr(__strtmp0);
    __strtmp0;
}); printf("%s", __temp); }
    __cleanup_all();
    return 0;
}
