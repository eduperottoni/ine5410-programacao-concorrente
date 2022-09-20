#include <stdio.h>

int main() {

    char c = 'A';
    char *pc = &c;
    double d = 5.34;
    double* pd1, * pd2;
    *pc = 'B';
    pd1 = &d;
    pd2 = pd1;
    *pd1 = *pd2 * 2;

    printf("c = %c, pc = %p, d = %lf, pd1 = %p, pd2 = %p\n", c, pc, d, pd1, pd2);

    return 0;
}