#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!

int create_son() {
    pid_t pid = fork();
    if (pid == 0) // processo filho
        printf("Processo filho %d criado\n", getpid());
    else if (pid > 0) // processo pai
        printf("Processo pai criou %d\n", pid);
    else //Erro na criação
        printf("Erro na criação do processo\n");

    return pid;
}

int main(int argc, char** argv) {

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    pid_t pid = create_son();
    if (pid > 0){ // processo pai
        pid_t pid1 = create_son();
        if (pid1 > 0) { // processo pai
            while(wait(NULL) >= 0);
            printf("Processo pai finalizado!\n");   
        }
    }

    return 0;
}
