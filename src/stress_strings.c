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
    const char* esc1 = "Line1\nLine2";
    const char* esc2 = "Tab\tSeparated";
    const char* esc3 = "Quote: \"Hello\"";
    const char* esc4 = "Backslash: \\path\\to\\file";
    const char* esc5 = "Carriage\rReturn";
    { printf("%s", esc1); }
    { printf("%s", esc2); }
    { printf("%s", esc3); }
    { printf("%s", esc4); }
    { printf("%s", esc5); }
    double pi = 3.1415899999999999;
    double e = 2.71828;
    const char* complex1 = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    char* __left_str = ({
    char* __strtmp1 = malloc(1);
    __strtmp1[0] = '\0';
    char* __left_str = ({
    char* __strtmp2 = malloc(1);
    __strtmp2[0] = '\0';
    __strtmp2 = realloc(__strtmp2, strlen("Pi=") + 1);
    strcat(__strtmp2, "Pi=");
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
    __strtmp1 = realloc(__strtmp1, strlen(__strtmp1) + strlen(", E=") + 1);
    strcat(__strtmp1, ", E=");
    __register_ptr(__strtmp1);
    __strtmp1;
});
    __strtmp0 = realloc(__strtmp0, strlen(__left_str) + 1);
    strcat(__strtmp0, __left_str);
    char* __right_str = ({
    char* __strtmp4 = malloc(64);
    sprintf(__strtmp4, "%.15g", (double)(e));
    __strtmp4;
});
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(__right_str) + 1);
    strcat(__strtmp0, __right_str);
    free(__right_str);
    __register_ptr(__strtmp0);
    __strtmp0;
});
    const char* complex2 = ({
    char* __strtmp5 = malloc(1);
    __strtmp5[0] = '\0';
    char* __left_str = ({
    char* __strtmp6 = malloc(1);
    __strtmp6[0] = '\0';
    char* __left_str = ({
    char* __strtmp7 = malloc(1);
    __strtmp7[0] = '\0';
    char* __left_str = ({
    char* __strtmp8 = malloc(1);
    __strtmp8[0] = '\0';
    __strtmp8 = realloc(__strtmp8, strlen("Math: ") + 1);
    strcat(__strtmp8, "Math: ");
    char* __right_str = ({
    char* __strtmp9 = malloc(64);
    sprintf(__strtmp9, "%.15g", (double)(pi));
    __strtmp9;
});
    __strtmp8 = realloc(__strtmp8, strlen(__strtmp8) + strlen(__right_str) + 1);
    strcat(__strtmp8, __right_str);
    free(__right_str);
    __register_ptr(__strtmp8);
    __strtmp8;
});
    __strtmp7 = realloc(__strtmp7, strlen(__left_str) + 1);
    strcat(__strtmp7, __left_str);
    __strtmp7 = realloc(__strtmp7, strlen(__strtmp7) + strlen(" and ") + 1);
    strcat(__strtmp7, " and ");
    __register_ptr(__strtmp7);
    __strtmp7;
});
    __strtmp6 = realloc(__strtmp6, strlen(__left_str) + 1);
    strcat(__strtmp6, __left_str);
    char* __right_str = ({
    char* __strtmp10 = malloc(64);
    sprintf(__strtmp10, "%.15g", (double)(e));
    __strtmp10;
});
    __strtmp6 = realloc(__strtmp6, strlen(__strtmp6) + strlen(__right_str) + 1);
    strcat(__strtmp6, __right_str);
    free(__right_str);
    __register_ptr(__strtmp6);
    __strtmp6;
});
    __strtmp5 = realloc(__strtmp5, strlen(__left_str) + 1);
    strcat(__strtmp5, __left_str);
    __strtmp5 = realloc(__strtmp5, strlen(__strtmp5) + strlen(" are constants") + 1);
    strcat(__strtmp5, " are constants");
    __register_ptr(__strtmp5);
    __strtmp5;
});
    const char* complex3 = ({
    char* __strtmp11 = malloc(1);
    __strtmp11[0] = '\0';
    char* __left_str = ({
    char* __strtmp12 = malloc(1);
    __strtmp12[0] = '\0';
    char* __left_str = ({
    char* __strtmp13 = malloc(1);
    __strtmp13[0] = '\0';
    char* __left_str = ({
    char* __strtmp14 = malloc(1);
    __strtmp14[0] = '\0';
    char* __left_str = ({
    char* __strtmp15 = malloc(64);
    sprintf(__strtmp15, "%.15g", (double)(pi));
    __strtmp15;
});
    __strtmp14 = realloc(__strtmp14, strlen(__left_str) + 1);
    strcat(__strtmp14, __left_str);
    free(__left_str);
    __strtmp14 = realloc(__strtmp14, strlen(__strtmp14) + strlen("+") + 1);
    strcat(__strtmp14, "+");
    __register_ptr(__strtmp14);
    __strtmp14;
});
    __strtmp13 = realloc(__strtmp13, strlen(__left_str) + 1);
    strcat(__strtmp13, __left_str);
    char* __right_str = ({
    char* __strtmp16 = malloc(64);
    sprintf(__strtmp16, "%.15g", (double)(e));
    __strtmp16;
});
    __strtmp13 = realloc(__strtmp13, strlen(__strtmp13) + strlen(__right_str) + 1);
    strcat(__strtmp13, __right_str);
    free(__right_str);
    __register_ptr(__strtmp13);
    __strtmp13;
});
    __strtmp12 = realloc(__strtmp12, strlen(__left_str) + 1);
    strcat(__strtmp12, __left_str);
    __strtmp12 = realloc(__strtmp12, strlen(__strtmp12) + strlen("=") + 1);
    strcat(__strtmp12, "=");
    __register_ptr(__strtmp12);
    __strtmp12;
});
    __strtmp11 = realloc(__strtmp11, strlen(__left_str) + 1);
    strcat(__strtmp11, __left_str);
    char* __right_str = ({
    char* __strtmp17 = malloc(64);
    sprintf(__strtmp17, "%.15g", (double)((pi + e)));
    __strtmp17;
});
    __strtmp11 = realloc(__strtmp11, strlen(__strtmp11) + strlen(__right_str) + 1);
    strcat(__strtmp11, __right_str);
    free(__right_str);
    __register_ptr(__strtmp11);
    __strtmp11;
});
    { printf("%s", complex1); }
    { printf("%s", complex2); }
    { printf("%s", complex3); }
    const char* long1 = "This is a very long string that tests the memory allocation and reallocation capabilities of the string concatenation system in our MINI compiler.";
    const char* long2 = ({
    char* __strtmp18 = malloc(1);
    __strtmp18[0] = '\0';
    char* __left_str = ({
    char* __strtmp19 = malloc(1);
    __strtmp19[0] = '\0';
    __strtmp19 = realloc(__strtmp19, strlen(long1) + 1);
    strcat(__strtmp19, long1);
    __strtmp19 = realloc(__strtmp19, strlen(__strtmp19) + strlen(" ") + 1);
    strcat(__strtmp19, " ");
    __register_ptr(__strtmp19);
    __strtmp19;
});
    __strtmp18 = realloc(__strtmp18, strlen(__left_str) + 1);
    strcat(__strtmp18, __left_str);
    __strtmp18 = realloc(__strtmp18, strlen(__strtmp18) + strlen(long1) + 1);
    strcat(__strtmp18, long1);
    __register_ptr(__strtmp18);
    __strtmp18;
});
    const char* long3 = ({
    char* __strtmp20 = malloc(1);
    __strtmp20[0] = '\0';
    char* __left_str = ({
    char* __strtmp21 = malloc(1);
    __strtmp21[0] = '\0';
    __strtmp21 = realloc(__strtmp21, strlen(long2) + 1);
    strcat(__strtmp21, long2);
    __strtmp21 = realloc(__strtmp21, strlen(__strtmp21) + strlen(" ") + 1);
    strcat(__strtmp21, " ");
    __register_ptr(__strtmp21);
    __strtmp21;
});
    __strtmp20 = realloc(__strtmp20, strlen(__left_str) + 1);
    strcat(__strtmp20, __left_str);
    __strtmp20 = realloc(__strtmp20, strlen(__strtmp20) + strlen(long2) + 1);
    strcat(__strtmp20, long2);
    __register_ptr(__strtmp20);
    __strtmp20;
});
    { printf("%s", long3); }
    const char* empty = "";
    const char* withEmpty = ({
    char* __strtmp22 = malloc(1);
    __strtmp22[0] = '\0';
    char* __left_str = ({
    char* __strtmp23 = malloc(1);
    __strtmp23[0] = '\0';
    __strtmp23 = realloc(__strtmp23, strlen(empty) + 1);
    strcat(__strtmp23, empty);
    __strtmp23 = realloc(__strtmp23, strlen(__strtmp23) + strlen("NotEmpty") + 1);
    strcat(__strtmp23, "NotEmpty");
    __register_ptr(__strtmp23);
    __strtmp23;
});
    __strtmp22 = realloc(__strtmp22, strlen(__left_str) + 1);
    strcat(__strtmp22, __left_str);
    __strtmp22 = realloc(__strtmp22, strlen(__strtmp22) + strlen(empty) + 1);
    strcat(__strtmp22, empty);
    __register_ptr(__strtmp22);
    __strtmp22;
});
    { printf("%s", withEmpty); }
    const char* var1 = "Hello";
    const char* var2 = "World";
    const char* mixed = ({
    char* __strtmp24 = malloc(1);
    __strtmp24[0] = '\0';
    char* __left_str = ({
    char* __strtmp25 = malloc(1);
    __strtmp25[0] = '\0';
    char* __left_str = ({
    char* __strtmp26 = malloc(1);
    __strtmp26[0] = '\0';
    __strtmp26 = realloc(__strtmp26, strlen(var1) + 1);
    strcat(__strtmp26, var1);
    __strtmp26 = realloc(__strtmp26, strlen(__strtmp26) + strlen(" ") + 1);
    strcat(__strtmp26, " ");
    __register_ptr(__strtmp26);
    __strtmp26;
});
    __strtmp25 = realloc(__strtmp25, strlen(__left_str) + 1);
    strcat(__strtmp25, __left_str);
    __strtmp25 = realloc(__strtmp25, strlen(__strtmp25) + strlen(var2) + 1);
    strcat(__strtmp25, var2);
    __register_ptr(__strtmp25);
    __strtmp25;
});
    __strtmp24 = realloc(__strtmp24, strlen(__left_str) + 1);
    strcat(__strtmp24, __left_str);
    __strtmp24 = realloc(__strtmp24, strlen(__strtmp24) + strlen("!") + 1);
    strcat(__strtmp24, "!");
    __register_ptr(__strtmp24);
    __strtmp24;
});
    { printf("%s", mixed); }
    __cleanup_all();
    return 0;
}
