#include <stdio.h>
#include "lib/math/math.h"
#include "lib/hello/hi/hi.h"

int main() {
    printf("this is page_size\n");
    int a = 123;
    int b = 345;
    printf("%d + %d = %d\n", a, b, add(a, b));
    vietnamese("Tai");
    return 0;
}
