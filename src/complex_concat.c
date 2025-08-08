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
    double pi;
    double scientific;
    const char* greeting;
    pi = 3.1415899999999999;
    scientific = 6.0220000000000003e+23;
    greeting = strdup("Hello"); __register_ptr(greeting);
    const char* simple;
    simple = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    __strtmp0 = realloc(__strtmp0, 6 + 1);
    strcat(__strtmp0, "Pi is ");
    char* __right_str = ({
    char* __strtmp1 = malloc(64);
    sprintf(__strtmp1, "%.15g", (double)(pi));
    __strtmp1;
});
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(__right_str) + 1);
    strcat(__strtmp0, __right_str);
    free(__right_str);
    __register_ptr(__strtmp0);
    __strtmp0;
}); __register_ptr(simple);
    const char* complex;
    complex = ({
    char* __strtmp2 = malloc(1);
    __strtmp2[0] = '\0';
    char* __left_str = ({
    char* __strtmp3 = malloc(1);
    __strtmp3[0] = '\0';
    char* __left_str = ({
    char* __strtmp4 = malloc(1);
    __strtmp4[0] = '\0';
    __strtmp4 = realloc(__strtmp4, 6 + 1);
    strcat(__strtmp4, "Pi is ");
    char* __right_str = ({
    char* __strtmp5 = malloc(64);
    sprintf(__strtmp5, "%.15g", (double)(pi));
    __strtmp5;
});
    __strtmp4 = realloc(__strtmp4, strlen(__strtmp4) + strlen(__right_str) + 1);
    strcat(__strtmp4, __right_str);
    free(__right_str);
    __register_ptr(__strtmp4);
    __strtmp4;
});
    __strtmp3 = realloc(__strtmp3, strlen(__left_str) + 1);
    strcat(__strtmp3, __left_str);
    __strtmp3 = realloc(__strtmp3, strlen(__strtmp3) + 17 + 1);
    strcat(__strtmp3, " and Avogadro is ");
    __register_ptr(__strtmp3);
    __strtmp3;
});
    __strtmp2 = realloc(__strtmp2, strlen(__left_str) + 1);
    strcat(__strtmp2, __left_str);
    char* __right_str = ({
    char* __strtmp6 = malloc(64);
    sprintf(__strtmp6, "%.15g", (double)(scientific));
    __strtmp6;
});
    __strtmp2 = realloc(__strtmp2, strlen(__strtmp2) + strlen(__right_str) + 1);
    strcat(__strtmp2, __right_str);
    free(__right_str);
    __register_ptr(__strtmp2);
    __strtmp2;
}); __register_ptr(complex);
    const char* veryComplex;
    veryComplex = ({
    char* __strtmp7 = malloc(1);
    __strtmp7[0] = '\0';
    char* __left_str = ({
    char* __strtmp8 = malloc(1);
    __strtmp8[0] = '\0';
    char* __left_str = ({
    char* __strtmp9 = malloc(1);
    __strtmp9[0] = '\0';
    char* __left_str = ({
    char* __strtmp10 = malloc(1);
    __strtmp10[0] = '\0';
    char* __left_str = ({
    char* __strtmp11 = malloc(1);
    __strtmp11[0] = '\0';
    __strtmp11 = realloc(__strtmp11, strlen(greeting) + 1);
    strcat(__strtmp11, greeting);
    __strtmp11 = realloc(__strtmp11, strlen(__strtmp11) + 8 + 1);
    strcat(__strtmp11, " world! ");
    __register_ptr(__strtmp11);
    __strtmp11;
});
    __strtmp10 = realloc(__strtmp10, strlen(__left_str) + 1);
    strcat(__strtmp10, __left_str);
    char* __right_str = ({
    char* __strtmp12 = malloc(64);
    sprintf(__strtmp12, "%.15g", (double)(pi));
    __strtmp12;
});
    __strtmp10 = realloc(__strtmp10, strlen(__strtmp10) + strlen(__right_str) + 1);
    strcat(__strtmp10, __right_str);
    free(__right_str);
    __register_ptr(__strtmp10);
    __strtmp10;
});
    __strtmp9 = realloc(__strtmp9, strlen(__left_str) + 1);
    strcat(__strtmp9, __left_str);
    __strtmp9 = realloc(__strtmp9, strlen(__strtmp9) + 11 + 1);
    strcat(__strtmp9, " is pi and ");
    __register_ptr(__strtmp9);
    __strtmp9;
});
    __strtmp8 = realloc(__strtmp8, strlen(__left_str) + 1);
    strcat(__strtmp8, __left_str);
    char* __right_str = ({
    char* __strtmp13 = malloc(64);
    sprintf(__strtmp13, "%.15g", (double)(scientific));
    __strtmp13;
});
    __strtmp8 = realloc(__strtmp8, strlen(__strtmp8) + strlen(__right_str) + 1);
    strcat(__strtmp8, __right_str);
    free(__right_str);
    __register_ptr(__strtmp8);
    __strtmp8;
});
    __strtmp7 = realloc(__strtmp7, strlen(__left_str) + 1);
    strcat(__strtmp7, __left_str);
    __strtmp7 = realloc(__strtmp7, strlen(__strtmp7) + 22 + 1);
    strcat(__strtmp7, " is Avogadro's number.");
    __register_ptr(__strtmp7);
    __strtmp7;
}); __register_ptr(veryComplex);
    { char* __temp = ({
    char* __strtmp14 = malloc(1);
    __strtmp14[0] = '\0';
    __strtmp14 = realloc(__strtmp14, strlen(simple) + 1);
    strcat(__strtmp14, simple);
    __strtmp14 = realloc(__strtmp14, strlen(__strtmp14) + 1 + 1);
    strcat(__strtmp14, "\n");
    __register_ptr(__strtmp14);
    __strtmp14;
}); printf("%s", __temp); }
    { char* __temp = ({
    char* __strtmp15 = malloc(1);
    __strtmp15[0] = '\0';
    __strtmp15 = realloc(__strtmp15, strlen(complex) + 1);
    strcat(__strtmp15, complex);
    __strtmp15 = realloc(__strtmp15, strlen(__strtmp15) + 1 + 1);
    strcat(__strtmp15, "\n");
    __register_ptr(__strtmp15);
    __strtmp15;
}); printf("%s", __temp); }
    { char* __temp = ({
    char* __strtmp16 = malloc(1);
    __strtmp16[0] = '\0';
    __strtmp16 = realloc(__strtmp16, strlen(veryComplex) + 1);
    strcat(__strtmp16, veryComplex);
    __strtmp16 = realloc(__strtmp16, strlen(__strtmp16) + 1 + 1);
    strcat(__strtmp16, "\n");
    __register_ptr(__strtmp16);
    __strtmp16;
}); printf("%s", __temp); }
    __cleanup_all();
    return 0;
}
