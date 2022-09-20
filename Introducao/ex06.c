#include <stdio.h>

int mult(int a, int b){
    return a * b;
}

int main() {
    int x = 10, y = 5;

    printf("resultado: %d\n", mult(x+2, y+5));

    return 0;
}