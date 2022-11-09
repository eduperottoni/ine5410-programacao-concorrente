#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

    if(argc != 2) {
        printf("Sintaxe: %s <numero de filhos>\n", argv[0]);
        return 1;
    }

    int nforks = atoi(argv[1]);

    /* Cria nforks processos filhos a partir do processo inicial (processo pai). */
    for (int i = 0; i < nforks; i++) {
        if(fork() == 0) {
            printf("Processo %d criado!\n", getpid());
            fflush(stdout);
            /* Garante que o filho criado não executa as próximas iterações do for. */
            break;
        }
    }

    while(wait(NULL) >= 0);

    return 0;
}