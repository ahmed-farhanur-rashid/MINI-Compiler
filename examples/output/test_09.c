#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ------- Auto-cleanup registry ------- */
static void **__cleanup_ptrs = NULL;
static int __cleanup_count = 0;
static int __cleanup_cap = 0;
static void __register_ptr(void *p){
    if(!p) return;
    if(__cleanup_count >= __cleanup_cap){
        __cleanup_cap = __cleanup_cap ? __cleanup_cap*2 : 16;
        __cleanup_ptrs = (void**)realloc(__cleanup_ptrs, (size_t)__cleanup_cap * sizeof(void*));
    }
    __cleanup_ptrs[__cleanup_count++] = p;
}
static void __cleanup_all(void){
    for(int i=0;i<__cleanup_count;i++) free(__cleanup_ptrs[i]);
    free(__cleanup_ptrs);
}

/* ------- Number formatting (double) ------- */
static void __trim_trailing_zeros(char *s){
    if(!s) return;
    char *dot = strchr(s, '.');
    if(!dot) return;
    char *p = s + strlen(s) - 1;
    while(p > dot && *p == '0'){ *p-- = '\0'; }
    if(p == dot) *p = '\0';
}
static char* __num_to_string(double v){
    char *buf = (char*)malloc(72);
    if(!buf) return NULL;
    /* 15 significant digits → like your original toString */
    snprintf(buf, 72, "%.15g", v);
    __trim_trailing_zeros(buf);
    return buf;
}

/* ------- String helpers that always return newly-allocated memory ------- */
static char* __dup_cstr(const char *s){
    if(!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if(!p) return NULL;
    memcpy(p, s, n);
    return p;
}
static char* __concat2(const char *a, const char *b){
    size_t na = a ? strlen(a) : 0;
    size_t nb = b ? strlen(b) : 0;
    char *p = (char*)malloc(na + nb + 1);
    if(!p) return NULL;
    if(a) memcpy(p, a, na); if(b) memcpy(p+na, b, nb);
    p[na+nb] = '\0';
    return p;
}

int main(void){
    char* s1;
    char* s2;
    s1 = (__dup_cstr("abc"));
    __register_ptr(s1);
    s2 = (__dup_cstr("def"));
    __register_ptr(s2);
    double x = (99);
    char* output = NULL;
    {
        /* build concatenation */
        size_t __need = strlen(s1) + 32 + strlen(s2) + 1;
        char *__strtmp3000 = (char*)malloc(__need);
        if(__strtmp3000){ __strtmp3000[0]='\0'; }
        if(s1) strcat(__strtmp3000, s1);
        char __numstr0[64]; sprintf(__numstr0, "1.06231149364103e-311", (double)(x)); strcat(__strtmp3000, __numstr0);
        if(s2) strcat(__strtmp3000, s2);
        output = __strtmp3000;
        __register_ptr(output);
    }
    { fputs(output ? output : "", stdout); }
    __cleanup_all();
    return 0;
}
