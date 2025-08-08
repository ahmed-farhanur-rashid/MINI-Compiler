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
    const char* name;
    double age;
    { printf("%s", "Enter your name: "); }
    char __buffer[256];
fgets(__buffer, 256, stdin);
__buffer[strcspn(__buffer, "\n")] = 0;
name = strdup(__buffer);
    { printf("%s", "Enter your age: "); }
    { double __temp; scanf("%lf", &__temp); age = __temp; }
int __c; while ((__c = getchar()) != '\n' && __c != EOF) { }
    { char* __temp = ({
    char* __strtmp0 = malloc(1);
    __strtmp0[0] = '\0';
    char* __left_str = ({
    char* __strtmp1 = malloc(1);
    __strtmp1[0] = '\0';
    char* __left_str = ({
    char* __strtmp2 = malloc(1);
    __strtmp2[0] = '\0';
    char* __left_str = ({
    char* __strtmp3 = malloc(1);
    __strtmp3[0] = '\0';
    __strtmp3 = realloc(__strtmp3, strlen("Hello ") + 1);
    strcat(__strtmp3, "Hello ");
    __strtmp3 = realloc(__strtmp3, strlen(__strtmp3) + strlen(name) + 1);
    strcat(__strtmp3, name);
    __register_ptr(__strtmp3);
    __strtmp3;
});
    __strtmp2 = realloc(__strtmp2, strlen(__left_str) + 1);
    strcat(__strtmp2, __left_str);
    __strtmp2 = realloc(__strtmp2, strlen(__strtmp2) + strlen(", you are ") + 1);
    strcat(__strtmp2, ", you are ");
    __register_ptr(__strtmp2);
    __strtmp2;
});
    __strtmp1 = realloc(__strtmp1, strlen(__left_str) + 1);
    strcat(__strtmp1, __left_str);
    char* __right_str = ({
    char* __strtmp4 = malloc(64);
    sprintf(__strtmp4, "%.15g", (double)(age));
    __strtmp4;
});
    __strtmp1 = realloc(__strtmp1, strlen(__strtmp1) + strlen(__right_str) + 1);
    strcat(__strtmp1, __right_str);
    free(__right_str);
    __register_ptr(__strtmp1);
    __strtmp1;
});
    __strtmp0 = realloc(__strtmp0, strlen(__left_str) + 1);
    strcat(__strtmp0, __left_str);
    __strtmp0 = realloc(__strtmp0, strlen(__strtmp0) + strlen(" years old") + 1);
    strcat(__strtmp0, " years old");
    __register_ptr(__strtmp0);
    __strtmp0;
}); printf("%s", __temp); }
    __cleanup_all();
    return 0;
}
