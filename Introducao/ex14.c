#include <stdio.h>
#include <stdlib.h>

    typedef struct{
        int idade;
        char* nome; 
    }pessoa;

    void present(pessoa p){
        printf("Nome: %s | idade: %d\n", p.nome, p.idade);
    }

int main(void) {

    pessoa joice;
    joice.nome = "Joice de Souza";
    joice.idade = 22;

    pessoa* eduardo = (pessoa*) malloc(sizeof(pessoa));
    eduardo -> idade = 20;
    eduardo -> nome = "Eduardo Perottoni";

    present(joice);

    return 0;
}