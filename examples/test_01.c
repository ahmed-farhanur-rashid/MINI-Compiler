#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    long double x;
    x = 123.456000;
    printf("%f\n", (double)x);
    const char* s;
    s = strdup("Hello, World!");
    printf("%s\n", s);
    return 0;
}
