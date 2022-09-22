#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

void create_grandson(int number) {
    for (int i = 0; i < number; i++){
        fflush(stdout);
        pid_t pid = fork();
        if (pid == 0) { // processo filho
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            sleep(5);
            printf("Processo %d finalizado\n", getpid());
            exit(0);
        } else if (pid < 0) { // Erro na criação
            printf("Erro na criação do processo\n");
        }
    }
}

void create_son(int number) {
    for (int i = 0; i < number; i++){
        pid_t pid = fork();
        if (pid == 0) {
            printf("Processo %d, filho de %d\n", getpid(), getppid());
            fflush(stdout);
            create_grandson(3);
            while(wait(NULL) >= 0);
            printf("Processo %d finalizado\n", getpid());
            exit(0);
            break;
        } else if (pid < 0) { // Erro na criação
            printf("Erro na criação do processo");
        }
    }
}

int main(int argc, char** argv) {

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/
    create_son(2);

    while(wait(NULL) >= 0);
    printf("Processo principal %d finalizado\n", getpid());    
    return 0;
}
