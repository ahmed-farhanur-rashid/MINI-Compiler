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
    double x = 42;
    const char* s = "Hello";
    { char __buf[64]; sprintf(__buf, "%.15g", x); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { printf("%s", s); }
    const char* result1 = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    __strtmp0 = realloc(__strtmp0, strlen(s) + 1);
    strcat(__strtmp0, s);
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(" World") + 1);
    strcat(__strtmp0, " World");
    __register_ptr(__strtmp0);
    __strtmp0;
});
    { printf("%s", result1); }
    const char* result2 = ({
    char* __strtmp1 = malloc(64);
    sprintf(__strtmp1, "%.15g", (double)(x));
    __strtmp1;
});
    { printf("%s", result2); }
    const char* result3 = ({
    char* __strtmp2 = malloc(1);
    __strtmp2[0] = '\0';
    char* __left_str = ({
    char* __strtmp3 = malloc(1);
    __strtmp3[0] = '\0';
    __strtmp3 = realloc(__strtmp3, strlen(s) + 1);
    strcat(__strtmp3, s);
    __strtmp3 = realloc(__strtmp3, strlen(__strtmp3) + strlen(" ") + 1);
    strcat(__strtmp3, " ");
    __register_ptr(__strtmp3);
    __strtmp3;
});
    __strtmp2 = realloc(__strtmp2, strlen(__left_str) + 1);
    strcat(__strtmp2, __left_str);
    char* __right_str = ({
    char* __strtmp4 = malloc(64);
    sprintf(__strtmp4, "%.15g", (double)(x));
    __strtmp4;
});
    __strtmp2 = realloc(__strtmp2, strlen(__strtmp2) + strlen(__right_str) + 1);
    strcat(__strtmp2, __right_str);
    free(__right_str);
    __register_ptr(__strtmp2);
    __strtmp2;
});
    { printf("%s", result3); }
    __cleanup_all();
    return 0;
}
