#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    float age;
    const char* str = "My name is ";
    const char* name;
    scanf("%f", &age);
int __c; while ((__c = getchar()) != '\n' && __c != EOF) { }
    char __buffer[256];
fgets(__buffer, 256, stdin);
__buffer[strcspn(__buffer, "\n")] = 0;
name = strdup(__buffer);
    char* __strtmp0 = malloc(strlen(str) + strlen(name) + 1);
    strcpy(__strtmp0, str);
    strcat(__strtmp0, name);
    const char* output = __strtmp0;
    printf("%s\n", output);
    float y;
    for (int __i = 0; __i < age; __i++) {
        scanf("%f", &y);
int __c; while ((__c = getchar()) != '\n' && __c != EOF) { }
    }
    return 0;
}
