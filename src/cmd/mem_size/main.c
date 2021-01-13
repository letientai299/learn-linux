#include <stdio.h>
#include "lib/math/math.h" // NOLINT(modernize-deprecated-headers)
#include "lib/hello/hi/hi.h"

int main() {
    printf("this is mem_size\n");
    int a = 123;
    int b = 345;
    printf("%d - %d = %d\n", a, b, sub(a, b));
    english("Tai");
    return 0;
}
