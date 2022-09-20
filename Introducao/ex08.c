#include <stdio.h>

int main() {

    int x = 10;

    int* ptr;
    ptr = &x;

    int y;
    y = *ptr;

    printf("X: %d PTR: %d Y: %d\n", x, ptr, y);

    return 0;
}