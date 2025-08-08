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
    int numbers_size = 3;
    int numbers_capacity = numbers_size;
    int numbers_count = 0;
    numbers = malloc(sizeof(double) * numbers_capacity);
    __register_ptr(numbers);
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 100;
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 200;
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 300;
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 400;
    // numbers.push_back()
    if (numbers_count >= numbers_capacity) {
        numbers_capacity *= 2;
        numbers = realloc(numbers, sizeof(double) * numbers_capacity);
    }
    numbers[numbers_count++] = 500;
    const char* size_msg = "List size: ";
    { printf("%s", size_msg); }
    { printf("%g", (double)(numbers_count)); }
    const char* msg = "Elements: ";
    { printf("%s", msg); }
    double i = 0;
    for (int __i = 0; __i < numbers_count; __i++) {
        { printf("%g", (double)(numbers[(int)(i)])); }
        i = (i + 1);
    }
    // List declaration: words
    char* *words;
    int words_size = 2;
    int words_capacity = words_size;
    int words_count = 0;
    words = malloc(sizeof(char*) * words_capacity);
    __register_ptr(words);
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
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "from";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "MINI";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "language";
    const char* word_msg = "Words: ";
    { printf("%s", word_msg); }
    double j = 0;
    for (int __i = 0; __i < words_count; __i++) {
        { printf("%s", words[(int)(j)]); }
        j = (j + 1);
    }
    __cleanup_all();
    return 0;
}
