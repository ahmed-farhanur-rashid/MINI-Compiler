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
    double x = 42.5;
    const char* s = "hello";
    const char* num_as_string = ({
    char* __strtmp0 = malloc(64);
    sprintf(__strtmp0, "%.15g", (double)(x));
    __strtmp0;
});
    { printf("%s", num_as_string); }
    double string_as_num = atof("123.45");
    { char __buf[64]; sprintf(__buf, "%.15g", string_as_num); char* __p = __buf + strlen(__buf) - 1; while(__p > __buf && *__p == '0' && __p > strchr(__buf, '.')) *__p-- = '\0'; if(__p > __buf && *__p == '.') *__p = '\0'; printf("%s", __buf); }
    // List declaration: nums
    double *nums;
    int nums_size = 2;
    int nums_capacity = nums_size;
    int nums_count = 0;
    nums = malloc(sizeof(double) * nums_capacity);
    __register_ptr(nums);
    // nums.push_back()
    if (nums_count >= nums_capacity) {
        nums_capacity *= 2;
        nums = realloc(nums, sizeof(double) * nums_capacity);
    }
    nums[nums_count++] = 100;
    { printf("%g", (double)(nums_count)); }
    __cleanup_all();
    return 0;
}
