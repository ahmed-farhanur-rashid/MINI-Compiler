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
    // List declaration: nums
    double *nums;
    int nums_size = 2;
    int nums_capacity = nums_size;
    int nums_count = 0;
    nums = malloc(sizeof(double) * nums_capacity);
    __register_ptr(nums);
    { printf("%g", (double)(nums_count)); }
    // nums.push_back()
    if (nums_count >= nums_capacity) {
        nums_capacity *= 2;
        nums = realloc(nums, sizeof(double) * nums_capacity);
    }
    nums[nums_count++] = 100.5;
    // nums.push_back()
    if (nums_count >= nums_capacity) {
        nums_capacity *= 2;
        nums = realloc(nums, sizeof(double) * nums_capacity);
    }
    nums[nums_count++] = 200.69999999999999;
    // nums.push_back()
    if (nums_count >= nums_capacity) {
        nums_capacity *= 2;
        nums = realloc(nums, sizeof(double) * nums_capacity);
    }
    nums[nums_count++] = 300.89999999999998;
    // nums.push_back()
    if (nums_count >= nums_capacity) {
        nums_capacity *= 2;
        nums = realloc(nums, sizeof(double) * nums_capacity);
    }
    nums[nums_count++] = 400.10000000000002;
    { printf("%g", (double)(nums_count)); }
    { printf("%g", (double)(nums[0])); }
    { printf("%g", (double)(nums[1])); }
    { printf("%g", (double)(nums[2])); }
    { printf("%g", (double)(nums[3])); }
    // nums.pop_back()
    if (nums_count > 0) nums_count--;
    { printf("%g", (double)(nums_count)); }
    // nums.pop_back()
    if (nums_count > 0) nums_count--;
    { printf("%g", (double)(nums_count)); }
    // List declaration: words
    char* *words;
    int words_size = 1;
    int words_capacity = words_size;
    int words_count = 0;
    words = malloc(sizeof(char*) * words_capacity);
    __register_ptr(words);
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "first";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "second";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "third";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "fourth";
    // words.push_back()
    if (words_count >= words_capacity) {
        words_capacity *= 2;
        words = realloc(words, sizeof(char*) * words_capacity);
    }
    words[words_count++] = "fifth";
    { printf("%g", (double)(words_count)); }
    { printf("%s", words[0]); }
    { printf("%s", words[1]); }
    { printf("%s", words[2]); }
    { printf("%s", words[3]); }
    { printf("%s", words[4]); }
    // words.pop_back()
    if (words_count > 0) words_count--;
    // words.pop_back()
    if (words_count > 0) words_count--;
    { printf("%g", (double)(words_count)); }
    __cleanup_all();
    return 0;
}
