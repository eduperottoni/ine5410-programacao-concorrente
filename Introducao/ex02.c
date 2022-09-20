#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc == 1) printf("Sem argumentos de entrada\n");
    else {
        for(int i = 0; i < argc; i++) {
            printf("agc[%d] = %s\n", i, argv[i]);
        }   
    }
    return 0;
}