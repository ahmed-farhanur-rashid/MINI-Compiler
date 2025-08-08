#include <stdio.h>

int main() {
    long double x = 123.456L;
    printf("Using %%Lf: %Lf\n", x);
    printf("Using %%f with cast: %f\n", (double)x);
    return 0;
}
