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
    double int_pos = 42;
    double int_neg = -123;
    double decimal = 3.1415899999999999;
    double neg_decimal = -2.71828;
    double scientific_pos = 6.0220000000000003e+23;
    double scientific_neg = -1.602e-19;
    double scientific_big = 12300000000;
    double scientific_small = 4.5599999999999998e-08;
    { char __buf[64]; sprintf(__buf, "%.15g", int_pos); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", int_neg); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", decimal); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", neg_decimal); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", scientific_pos); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", scientific_neg); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", scientific_big); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", scientific_small); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    double sum = (int_pos + int_neg);
    double diff = (decimal - neg_decimal);
    double product = (scientific_pos * scientific_neg);
    double quotient = (scientific_big / scientific_small);
    { char __buf[64]; sprintf(__buf, "%.15g", sum); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", diff); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", product); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", quotient); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    const char* str_int_pos = ({
    char* __strtmp0 = malloc(64);
    sprintf(__strtmp0, "%.15g", (double)(int_pos));
    __strtmp0;
});
    const char* str_int_neg = ({
    char* __strtmp1 = malloc(64);
    sprintf(__strtmp1, "%.15g", (double)(int_neg));
    __strtmp1;
});
    const char* str_decimal = ({
    char* __strtmp2 = malloc(64);
    sprintf(__strtmp2, "%.15g", (double)(decimal));
    __strtmp2;
});
    const char* str_scientific = ({
    char* __strtmp3 = malloc(64);
    sprintf(__strtmp3, "%.15g", (double)(scientific_pos));
    __strtmp3;
});
    { printf("%s", str_int_pos); }
    { printf("%s", str_int_neg); }
    { printf("%s", str_decimal); }
    { printf("%s", str_scientific); }
    double zero = 0;
    double tiny = 1.0000000000000001e-15;
    double huge = 1000000000000000;
    double precise = 123.45678912345601;
    { char __buf[64]; sprintf(__buf, "%.15g", zero); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", tiny); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", huge); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    { char __buf[64]; sprintf(__buf, "%.15g", precise); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    __cleanup_all();
    return 0;
}
