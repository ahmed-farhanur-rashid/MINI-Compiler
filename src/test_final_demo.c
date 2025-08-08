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
    const char* greeting = "Hello, World!\n";
    { printf("%s", greeting); }
    double pi = 3.1415899999999999;
    double negative = -42;
    double scientific = 6.02e+23;
    const char* result = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    char* __left_str = ({
    char* __strtmp1 = malloc(1);
    __strtmp1[0] = '\0';
    char* __left_str = ({
    char* __strtmp2 = malloc(1);
    __strtmp2[0] = '\0';
    __strtmp2 = realloc(__strtmp2, strlen("Pi is ") + 1);
    strcat(__strtmp2, "Pi is ");
    char* __right_str = ({
    char* __strtmp3 = malloc(64);
    sprintf(__strtmp3, "%.15g", (double)(pi));
    __strtmp3;
});
    __strtmp2 = realloc(__strtmp2, strlen(__strtmp2) + strlen(__right_str) + 1);
    strcat(__strtmp2, __right_str);
    free(__right_str);
    __register_ptr(__strtmp2);
    __strtmp2;
});
    __strtmp1 = realloc(__strtmp1, strlen(__left_str) + 1);
    strcat(__strtmp1, __left_str);
    __strtmp1 = realloc(__strtmp1, strlen(__strtmp1) + strlen(" and Avogadro is ") + 1);
    strcat(__strtmp1, " and Avogadro is ");
    __register_ptr(__strtmp1);
    __strtmp1;
});
    __strtmp0 = realloc(__strtmp0, strlen(__left_str) + 1);
    strcat(__strtmp0, __left_str);
    char* __right_str = ({
    char* __strtmp4 = malloc(64);
    sprintf(__strtmp4, "%.15g", (double)(scientific));
    __strtmp4;
});
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(__right_str) + 1);
    strcat(__strtmp0, __right_str);
    free(__right_str);
    __register_ptr(__strtmp0);
    __strtmp0;
});
    { printf("%s", result); }
    { printf("%s", "\n"); }
    if ((negative < 0)) {
        { printf("%s", "Negative numbers work: "); }
        { char* __temp = ({
    char* __strtmp5 = malloc(64);
    sprintf(__strtmp5, "%.15g", (double)(negative));
    __strtmp5;
}); printf("%s", __temp); }
        { printf("%s", "\n"); }
    }
    for (int __i = 0; __i < 3; __i++) {
        { printf("%s", "Memory management test "); }
    }
    { printf("%s", "\n"); }
    const char* escaped = "Tabs:\twork\nNewlines:\nwork\nQuotes:\"work\"";
    { printf("%s", escaped); }
    __cleanup_all();
    return 0;
}
