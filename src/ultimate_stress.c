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
    double pi = 3.1415926535900001;
    double e = -2.7182818279999998;
    double avogadro = 6.0220000000000003e+23;
    double electron = -1.602e-19;
    const char* greeting = "Hello";
    const char* world = "World";
    const char* empty = "";
    const char* simple_concat = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    char* __left_str = ({
    char* __strtmp1 = malloc(1);
    __strtmp1[0] = '\0';
    __strtmp1 = realloc(__strtmp1, strlen(greeting) + 1);
    strcat(__strtmp1, greeting);
    __strtmp1 = realloc(__strtmp1, strlen(__strtmp1) + strlen(" ") + 1);
    strcat(__strtmp1, " ");
    __register_ptr(__strtmp1);
    __strtmp1;
});
    __strtmp0 = realloc(__strtmp0, strlen(__left_str) + 1);
    strcat(__strtmp0, __left_str);
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(world) + 1);
    strcat(__strtmp0, world);
    __register_ptr(__strtmp0);
    __strtmp0;
});
    const char* complex_concat = ({
    char* __strtmp2 = malloc(1);
    __strtmp2[0] = '\0';
    char* __left_str = ({
    char* __strtmp3 = malloc(1);
    __strtmp3[0] = '\0';
    char* __left_str = ({
    char* __strtmp4 = malloc(1);
    __strtmp4[0] = '\0';
    __strtmp4 = realloc(__strtmp4, strlen("Pi=") + 1);
    strcat(__strtmp4, "Pi=");
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
    __strtmp3 = realloc(__strtmp3, strlen(__strtmp3) + strlen(", E=") + 1);
    strcat(__strtmp3, ", E=");
    __register_ptr(__strtmp3);
    __strtmp3;
});
    __strtmp2 = realloc(__strtmp2, strlen(__left_str) + 1);
    strcat(__strtmp2, __left_str);
    char* __right_str = ({
    char* __strtmp6 = malloc(64);
    sprintf(__strtmp6, "%.15g", (double)(e));
    __strtmp6;
});
    __strtmp2 = realloc(__strtmp2, strlen(__strtmp2) + strlen(__right_str) + 1);
    strcat(__strtmp2, __right_str);
    free(__right_str);
    __register_ptr(__strtmp2);
    __strtmp2;
});
    const char* mega_concat = ({
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
    char* __left_str = ({
    char* __strtmp12 = malloc(1);
    __strtmp12[0] = '\0';
    char* __left_str = ({
    char* __strtmp13 = malloc(1);
    __strtmp13[0] = '\0';
    __strtmp13 = realloc(__strtmp13, strlen(greeting) + 1);
    strcat(__strtmp13, greeting);
    __strtmp13 = realloc(__strtmp13, strlen(__strtmp13) + strlen(" ") + 1);
    strcat(__strtmp13, " ");
    __register_ptr(__strtmp13);
    __strtmp13;
});
    __strtmp12 = realloc(__strtmp12, strlen(__left_str) + 1);
    strcat(__strtmp12, __left_str);
    __strtmp12 = realloc(__strtmp12, strlen(__strtmp12) + strlen(world) + 1);
    strcat(__strtmp12, world);
    __register_ptr(__strtmp12);
    __strtmp12;
});
    __strtmp11 = realloc(__strtmp11, strlen(__left_str) + 1);
    strcat(__strtmp11, __left_str);
    __strtmp11 = realloc(__strtmp11, strlen(__strtmp11) + strlen("! ") + 1);
    strcat(__strtmp11, "! ");
    __register_ptr(__strtmp11);
    __strtmp11;
});
    __strtmp10 = realloc(__strtmp10, strlen(__left_str) + 1);
    strcat(__strtmp10, __left_str);
    char* __right_str = ({
    char* __strtmp14 = malloc(64);
    sprintf(__strtmp14, "%.15g", (double)(pi));
    __strtmp14;
});
    __strtmp10 = realloc(__strtmp10, strlen(__strtmp10) + strlen(__right_str) + 1);
    strcat(__strtmp10, __right_str);
    free(__right_str);
    __register_ptr(__strtmp10);
    __strtmp10;
});
    __strtmp9 = realloc(__strtmp9, strlen(__left_str) + 1);
    strcat(__strtmp9, __left_str);
    __strtmp9 = realloc(__strtmp9, strlen(__strtmp9) + strlen(" and ") + 1);
    strcat(__strtmp9, " and ");
    __register_ptr(__strtmp9);
    __strtmp9;
});
    __strtmp8 = realloc(__strtmp8, strlen(__left_str) + 1);
    strcat(__strtmp8, __left_str);
    char* __right_str = ({
    char* __strtmp15 = malloc(64);
    sprintf(__strtmp15, "%.15g", (double)(avogadro));
    __strtmp15;
});
    __strtmp8 = realloc(__strtmp8, strlen(__strtmp8) + strlen(__right_str) + 1);
    strcat(__strtmp8, __right_str);
    free(__right_str);
    __register_ptr(__strtmp8);
    __strtmp8;
});
    __strtmp7 = realloc(__strtmp7, strlen(__left_str) + 1);
    strcat(__strtmp7, __left_str);
    __strtmp7 = realloc(__strtmp7, strlen(__strtmp7) + strlen(" are important.") + 1);
    strcat(__strtmp7, " are important.");
    __register_ptr(__strtmp7);
    __strtmp7;
});
    const char* newlines = "Line1\nLine2\nLine3";
    const char* tabs = "Col1\tCol2\tCol3";
    const char* quotes = "He said \"Hello\" to her";
    const char* paths = "C:\\Users\\Name\\File.txt";
    double sum = (pi + e);
    double product = (avogadro * electron);
    double complex_math = ((pi * e) / (avogadro + electron));
    if ((pi > e)) {
        { printf("%s", "Pi is greater than E"); }
    }
    if ((electron < 0)) {
        { printf("%s", "Electron charge is negative"); }
    } else {
        { printf("%s", "This should not print"); }
    }
    for (int __i = 0; __i < 3; __i++) {
        { printf("%s", "Loop iteration"); }
    }
    { printf("%s", simple_concat); }
    { printf("%s", complex_concat); }
    { printf("%s", mega_concat); }
    { printf("%s", newlines); }
    { printf("%s", tabs); }
    { printf("%s", quotes); }
    { printf("%s", paths); }
    { char* __temp = ({
    char* __strtmp16 = malloc(64);
    sprintf(__strtmp16, "%.15g", (double)(sum));
    __strtmp16;
}); printf("%s", __temp); }
    { char* __temp = ({
    char* __strtmp17 = malloc(64);
    sprintf(__strtmp17, "%.15g", (double)(product));
    __strtmp17;
}); printf("%s", __temp); }
    { char* __temp = ({
    char* __strtmp18 = malloc(64);
    sprintf(__strtmp18, "%.15g", (double)(complex_math));
    __strtmp18;
}); printf("%s", __temp); }
    const char* pi_str = ({
    char* __strtmp19 = malloc(64);
    sprintf(__strtmp19, "%.15g", (double)(pi));
    __strtmp19;
});
    const char* e_str = ({
    char* __strtmp20 = malloc(64);
    sprintf(__strtmp20, "%.15g", (double)(e));
    __strtmp20;
});
    double pi_back = atof(pi_str);
    double e_back = atof(e_str);
    { char* __temp = ({
    char* __strtmp21 = malloc(1);
    __strtmp21[0] = '\0';
    char* __left_str = ({
    char* __strtmp22 = malloc(1);
    __strtmp22[0] = '\0';
    char* __left_str = ({
    char* __strtmp23 = malloc(1);
    __strtmp23[0] = '\0';
    __strtmp23 = realloc(__strtmp23, strlen("Converted back: ") + 1);
    strcat(__strtmp23, "Converted back: ");
    char* __right_str = ({
    char* __strtmp24 = malloc(64);
    sprintf(__strtmp24, "%.15g", (double)(pi_back));
    __strtmp24;
});
    __strtmp23 = realloc(__strtmp23, strlen(__strtmp23) + strlen(__right_str) + 1);
    strcat(__strtmp23, __right_str);
    free(__right_str);
    __register_ptr(__strtmp23);
    __strtmp23;
});
    __strtmp22 = realloc(__strtmp22, strlen(__left_str) + 1);
    strcat(__strtmp22, __left_str);
    __strtmp22 = realloc(__strtmp22, strlen(__strtmp22) + strlen(" and ") + 1);
    strcat(__strtmp22, " and ");
    __register_ptr(__strtmp22);
    __strtmp22;
});
    __strtmp21 = realloc(__strtmp21, strlen(__left_str) + 1);
    strcat(__strtmp21, __left_str);
    char* __right_str = ({
    char* __strtmp25 = malloc(64);
    sprintf(__strtmp25, "%.15g", (double)(e_back));
    __strtmp25;
});
    __strtmp21 = realloc(__strtmp21, strlen(__strtmp21) + strlen(__right_str) + 1);
    strcat(__strtmp21, __right_str);
    free(__right_str);
    __register_ptr(__strtmp21);
    __strtmp21;
}); printf("%s", __temp); }
    __cleanup_all();
    return 0;
}
