#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("Tamanho de um char: %d bytes\n", (int)sizeof(char));
    printf("Tamanho de um int: %d bytes\n", (int)sizeof(int));
    printf("Tamanho de um short int: %d bytes\n", (int)sizeof(short int));
    printf("Tamanho de um long int: %d bytes\n", (int)sizeof(long int));
    printf("Tamanho de um unsigned int: %d bytes\n", (int)sizeof(unsigned int));
    printf("Tamanho de um float: %d bytes\n", (int)sizeof(float));
    printf("Tamanho de um double: %d bytes\n", (int)sizeof(double));
    printf("Tamanho de um long double: %d bytes\n", (int)sizeof(long double));
}