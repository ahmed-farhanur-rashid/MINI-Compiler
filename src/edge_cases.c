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
    double huge = 1.7976931348623157e+308;
    { char __buf[64]; sprintf(__buf, "%.15g", huge); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    double tiny = 2.2250738585072014e-308;
    { char __buf[64]; sprintf(__buf, "%.15g", tiny); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    double zero = 0;
    double result1 = (zero + 100);
    double result2 = (100 * zero);
    { char __buf[64]; sprintf(__buf, "%.15g", result1); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", result2); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    const char* special = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
    { printf("%s", special); }
    const char* very_long_variable_name_to_test_identifier_limits = "test";
    { printf("%s", very_long_variable_name_to_test_identifier_limits); }
    double complex = (((5 + 3) * (10 - 2)) / 4);
    { char __buf[64]; sprintf(__buf, "%.15g", complex); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    const char* mixed = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    char* __left_str = ({
    char* __strtmp1 = malloc(1);
    __strtmp1[0] = '\0';
    __strtmp1 = realloc(__strtmp1, strlen("Value: ") + 1);
    strcat(__strtmp1, "Value: ");
    char* __right_str = ({
    char* __strtmp2 = malloc(64);
    sprintf(__strtmp2, "%.15g", (double)(42));
    __strtmp2;
});
    __strtmp1 = realloc(__strtmp1, strlen(__strtmp1) + strlen(__right_str) + 1);
    strcat(__strtmp1, __right_str);
    free(__right_str);
    __register_ptr(__strtmp1);
    __strtmp1;
});
    __strtmp0 = realloc(__strtmp0, strlen(__left_str) + 1);
    strcat(__strtmp0, __left_str);
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(" End") + 1);
    strcat(__strtmp0, " End");
    __register_ptr(__strtmp0);
    __strtmp0;
});
    { printf("%s", mixed); }
    __cleanup_all();
    return 0;
}
