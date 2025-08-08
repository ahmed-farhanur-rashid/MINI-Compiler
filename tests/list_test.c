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
    // List declaration: numbers
    double *numbers;
    int numbers_size = 5;
    int numbers_capacity = numbers_size;
    int numbers_count = 0;
    numbers = malloc(sizeof(double) * numbers_capacity);
    __register_ptr(numbers);
    // List declaration: words
    char* *words;
    int words_size = 3;
    int words_capacity = words_size;
    int words_count = 0;
    words = malloc(sizeof(char*) * words_capacity);
    __register_ptr(words);
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 10.5;
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 20.699999999999999;
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 30.899999999999999;
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "hello";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "world";
    { printf("%g", (double)(numbers_count)); }
    { printf("%g", (double)(words_count)); }
    { printf("%g", (double)(numbers[0])); }
    { printf("%g", (double)(numbers[1])); }
    { printf("%s", words[0]); }
    { printf("%s", words[1]); }
    // numbers.pop_back()
    if (numbers_count > 0) numbers_count--;
    { printf("%g", (double)(numbers_count)); }
    __cleanup_all();
    return 0;
}
