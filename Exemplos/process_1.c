#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {

    /* Cria um processo filho. */
    fork();
    
    printf("Processo %d executando!\n", getpid());
    fflush(stdout);

    while(wait(NULL) >= 0);
    
    return 0;
}