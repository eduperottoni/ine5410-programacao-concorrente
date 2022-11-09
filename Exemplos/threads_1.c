#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* thread(void* arg) {
    printf("Thread iniciada!\n");
    fflush(stdout);
    return 0;
}

int main(int argc, char** argv) {

    if(argc != 2) {
        printf("Sintaxe: %s <numero de threads>\n", argv[0]);
        return 1;
    }
    
    int n_threads = atoi(argv[1]);
    pthread_t threads[n_threads];
    
    /* Cria n_threads threads. */
    for (int i = 0; i < n_threads; ++i)
        pthread_create(&threads[i], NULL, thread, NULL);
    
    for (int i = 0; i < n_threads; ++i)
        pthread_join(threads[i], NULL);
    
    printf("Thread principal finalizada.\n");
        
    return 0;
}