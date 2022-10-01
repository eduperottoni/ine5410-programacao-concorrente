#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);

typedef struct {
    unsigned int step, threads_amount, start;
    double* array_op1; 
    double* array_op2;
    double* array_result;
} thread_infos;

void* sum_indexes(void* arg) {
    thread_infos *infos = (thread_infos*)arg;
    for (int i = infos->start; i < infos->threads_amount; i += infos->step)
        infos->array_result[i] = infos->array_op1[i] + infos->array_op2[i];
    pthread_exit(NULL);
}

void* sum_indexes(void* arg);

int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n",
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar
    if (a_size < n_threads) n_threads = a_size;
    pthread_t threads[n_threads];

    thread_infos threads_infos[n_threads];

    for (int i = 0; i < n_threads; i++){
        // Passo struct com a quantidade de threads e i da thread.
        // i da thread determina o índice inicial de cálculo pela thread
        // step é o passo de cada thread nos índices
        threads_infos[i].step = (unsigned int)n_threads;
        threads_infos[i].threads_amount = (unsigned int)a_size;
        threads_infos[i].array_op1 = a;
        threads_infos[i].array_op2 = b;
        threads_infos[i].array_result = c;
        threads_infos[i].start = (unsigned int)i;
    }

    for (int i = 0; i < n_threads; i++)
        pthread_create(&threads[i], NULL, sum_indexes, (void*)&threads_infos[i]);

    for (int i = 0; i < n_threads; i++)
        pthread_join(threads[i], NULL);

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);
    

    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
}
