#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Declara o mutex 
pthread_mutex_t mtx; // = PTHREAD_MUTEX_INITIALIZER;

// Código da thread 
void *sleeper(void *arg) {
    int my_id = *(int *)arg;
    printf("Thread %d iniciada.\n", my_id);
    pthread_mutex_lock(&mtx);
    printf("Thread %d obteve o mutex.\n", my_id);
    sleep(5);
    printf("Thread %d liberando o mutex.\n", my_id);
    pthread_mutex_unlock(&mtx);
    pthread_exit(NULL); 
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    pthread_t threads[n_threads];
    int thread_id[n_threads];

    // Inicializa mtx, caso ele não tenha sido inicializado com
    // PTHREAD_MUTEX_INITIALIZER
    pthread_mutex_init(&mtx, NULL);

    for (int i = 0; i < n_threads; i++) {
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, sleeper, (void*)&thread_id[i]);
    }

    for (int i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    // Destroy mtx. Qualquer uso futuro do mtx será um erro.
    pthread_mutex_destroy(&mtx);

    return 0;
}