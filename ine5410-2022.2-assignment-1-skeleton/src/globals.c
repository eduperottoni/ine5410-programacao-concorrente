#include <stdlib.h>

#include "globals.h"
#include "args.h"

/*
    VOCÊ DEVE CRIAR VARIÁVEIS GLOBAIS PARA ARMAZENAR DADOS SOBRE A SIMULAÇÃO.
    NOTAS:
    1.  OS SEGUINTES DADOS DEVEM SER ARMAZENADOS E PRINTADOS AO FIM DA SIMULAÇÃO:
        a. Quantidade de clientes que sentaram-se no conveyor e consumiram todos os itens desejados
        b. Quantidades produzidas de cada alimento
        c. Quantidades consumidas de cada alimento
    2.  SIGA OS EXEMPLOS DE VARIÁVEIS GLOBAIS JÁ EXISTENTES NESSE ARQUIVO PARA CRIAR AS NOVAS.
*/

/* VARIÁVEIS GLOBAIS CRIADAS */

dishes_info_t* global_dishes_info = NULL;
int global_served_customers = 0;
unsigned int global_opened = TRUE;

virtual_clock_t* global_virtual_clock = NULL;
conveyor_belt_t* global_conveyor_belt = NULL;
queue_t* global_queue = NULL;

pthread_mutex_t* global_consumed_dishes_mutexes = NULL;


pthread_mutex_t* global_consumed_dishes_mutexes_init(int menu_size) {
    pthread_mutex_t* self = malloc(sizeof(pthread_mutex_t)*menu_size);
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `pthread_mutex_t* global_consumed_dishes_mutexes_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < menu_size; i++)
        pthread_mutex_init(&self[i], NULL);
    return self;
}

void global_consumed_dishes_mutexes_finalize(pthread_mutex_t* self) {
    int size = sizeof(self) / sizeof(pthread_mutex_t);
    
    for (int i = 0; i < size; i++) {
        pthread_mutex_destroy(&self[i]);
        free(&self[i]);
    }
}

pthread_mutex_t* globals_get_consumed_dishes_mutexes() {
    return global_consumed_dishes_mutexes;
}

void globals_set_consumed_dishes_mutexes(pthread_mutex_t* mutexes) {
    global_consumed_dishes_mutexes = mutexes;
}

unsigned int globals_get_opened() {
    return global_opened;
}

void globals_set_opened(int opened) {
    global_opened = opened;
}

void globals_set_dishes_info(dishes_info_t* dishes_info) {
    global_dishes_info = dishes_info;
}

dishes_info_t* globals_get_dishes_info() {
    return global_dishes_info;
}

void globals_set_served_customers(int n) {
    global_served_customers = n;
}

int globals_get_served_customers() {
    return global_served_customers;
}

dishes_info_t* dishes_info_init(int menu_size) {
    dishes_info_t* self = malloc(sizeof(dishes_info_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `dishes_info_t* dishes_info_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    self -> consumed_dishes = (int*) malloc(sizeof(int)* menu_size);
    self -> prepared_dishes = (int*) malloc(sizeof(int)* menu_size);
    for (int i=0; i < menu_size; i++) {
        self -> prepared_dishes[i] = 0;
        self -> consumed_dishes[i] = 0;
    }
    return self;
}

void print_simulation_counters(dishes_info_t* info, int served_customers) {
    title();
    fprintf(stdout, MAGENTA "Simulation counters results:\n" NO_COLOR);
    fprintf(stdout, CYAN  " Total customers served with fulfilled wishes => %d" NO_COLOR "\n", served_customers);
    fprintf(stdout, CYAN  " Prepared dishes " NO_COLOR "\n");
    fprintf(stdout, GREEN "  Sushi                   " NO_COLOR "%d\n", info->prepared_dishes[0]);
    fprintf(stdout, GREEN "  Dango                   " NO_COLOR "%d\n", info->prepared_dishes[1]);
    fprintf(stdout, GREEN "  Ramen                   " NO_COLOR "%d\n", info->prepared_dishes[2]);
    fprintf(stdout, GREEN "  Onigiri                 " NO_COLOR "%d\n", info->prepared_dishes[3]);
    fprintf(stdout, GREEN "  Tofu                    " NO_COLOR "%d\n", info->prepared_dishes[4]);
    fprintf(stdout, CYAN  " Consumed dishes " NO_COLOR "\n");
    fprintf(stdout, GREEN "  Sushi                   " NO_COLOR "%d\n", info->consumed_dishes[0]);
    fprintf(stdout, GREEN "  Dango                   " NO_COLOR "%d\n", info->consumed_dishes[1]);
    fprintf(stdout, GREEN "  Ramen                   " NO_COLOR "%d\n", info->consumed_dishes[2]);
    fprintf(stdout, GREEN "  Onigiri                 " NO_COLOR "%d\n", info->consumed_dishes[3]);
    fprintf(stdout, GREEN "  Tofu                    " NO_COLOR "%d\n", info->consumed_dishes[4]);
    separator();
}

void dishes_info_finalize(dishes_info_t* self) {
    print_simulation_counters(self, global_served_customers);
    free(self->consumed_dishes);
    free(self->prepared_dishes);
    free(self);
}

void globals_set_virtual_clock(virtual_clock_t* virtual_clock) {
    global_virtual_clock = virtual_clock;
}

virtual_clock_t* globals_get_virtual_clock() {
    return global_virtual_clock;
}

void globals_set_conveyor_belt(conveyor_belt_t* conveyor_belt) {
    global_conveyor_belt = conveyor_belt;
}

conveyor_belt_t* globals_get_conveyor_belt() {
    return global_conveyor_belt;
}

void globals_set_queue(queue_t* queue) {
    global_queue = queue;
}

queue_t* globals_get_queue() {
    return global_queue;
}

/**
 * @brief Finaliza todas as variáveis globais.
 * Se criar alguma variável global que faça uso de mallocs, lembre-se sempre 
 * de usar o free dentro dessa função.
 */
void globals_finalize() {
    conveyor_belt_finalize(global_conveyor_belt);
    virtual_clock_finalize(global_virtual_clock);
    fprintf(stdout, GREEN "GLOBALS FINALIZED!\n" NO_COLOR);
    dishes_info_finalize(global_dishes_info);
    global_consumed_dishes_mutexes_finalize(global_consumed_dishes_mutexes);
}
