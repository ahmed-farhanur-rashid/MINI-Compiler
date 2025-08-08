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
    // List declaration: small_list
    double *small_list;
    int small_list_size = 2;
    int small_list_capacity = small_list_size;
    int small_list_count = 0;
    small_list = malloc(sizeof(double) * small_list_capacity);
    __register_ptr(small_list);
    // small_list.push_back()
    if (small_list_count >= small_list_capacity) {
        small_list_capacity *= 2;
        small_list = realloc(small_list, sizeof(double) * small_list_capacity);
    }
    small_list[small_list_count++] = 10;
    // small_list.push_back()
    if (small_list_count >= small_list_capacity) {
        small_list_capacity *= 2;
        small_list = realloc(small_list, sizeof(double) * small_list_capacity);
    }
    small_list[small_list_count++] = 20;
    // small_list.push_back()
    if (small_list_count >= small_list_capacity) {
        small_list_capacity *= 2;
        small_list = realloc(small_list, sizeof(double) * small_list_capacity);
    }
    small_list[small_list_count++] = 30;
    // small_list.push_back()
    if (small_list_count >= small_list_capacity) {
        small_list_capacity *= 2;
        small_list = realloc(small_list, sizeof(double) * small_list_capacity);
    }
    small_list[small_list_count++] = 40;
    // small_list.push_back()
    if (small_list_count >= small_list_capacity) {
        small_list_capacity *= 2;
        small_list = realloc(small_list, sizeof(double) * small_list_capacity);
    }
    small_list[small_list_count++] = 50;
    // small_list.push_back()
    if (small_list_count >= small_list_capacity) {
        small_list_capacity *= 2;
        small_list = realloc(small_list, sizeof(double) * small_list_capacity);
    }
    small_list[small_list_count++] = 60;
    { printf("%g", (double)(small_list_count)); }
    { printf("%g", (double)(small_list[0])); }
    { printf("%g", (double)(small_list[1])); }
    { printf("%g", (double)(small_list[2])); }
    { printf("%g", (double)(small_list[3])); }
    { printf("%g", (double)(small_list[4])); }
    { printf("%g", (double)(small_list[5])); }
    __cleanup_all();
    return 0;
}
