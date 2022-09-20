#include <stdio.h>

int main() {

    int vetor[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    int *y = vetor;

    for (int i = 0; i < 8; i++) {
        printf("posição %d: %d\n", i, *(y + i));
    }

    int* z;

    // Segmentation fault
    for (int i = 0; i < 8; i++) {
        z[i] = i;
    }


    return 0;
}