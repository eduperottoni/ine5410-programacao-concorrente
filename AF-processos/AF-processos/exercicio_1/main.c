#include <stdio.h>

#include <unistd.h>
#include <sys/wait.h>

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


int create_process() {
    pid_t pid = fork();
    
    if (pid == 0) { // processo filho
        printf("Processo filho %d criado\n", getpid());
    } else { // processo pai
        printf("Processo pai criou %d\n", pid);
    }

    return pid;
}

int main(int argc, char** argv) {

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/

    // Criando processo filho
    pid_t pid = create_process();
    if (pid > 0){
        pid_t pid = create_process();
        if (pid > 0) {
            while(wait(NULL) >= 0);
            printf("Processo pai finalizado!\n");   
        }
    }

    return 0;
}
