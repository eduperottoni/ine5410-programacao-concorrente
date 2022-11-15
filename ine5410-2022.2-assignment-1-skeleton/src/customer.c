#include <stdio.h>
#include <stdlib.h>

#include "virtual_clock.h"
#include "customer.h"
#include "globals.h"


void* customer_run(void* arg) {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  A PRIMEIRA AÇÃO REALIZADA SERÁ ESPERAR NA FILA GLOBAL DE CLIENTES, ATÉ QUE O HOSTESS
            GUIE O CLIENTE PARA UM ASSENTO LIVRE.
        2.  APÓS SENTAR-SE, O CLIENTE COMEÇARÁ PEGAR E COMER OS PRATOS DA ESTEIRA.
        3.  O CLIENTE SÓ PODERÁ PEGAR UM PRATO QUANDO A ESTEIRA ESTIVER PARADA.
        4.  O CLIENTE SÓ PEGARÁ PRATOS CASO ELE DESEJE-OS, INFORMAÇÃO CONTIDA NO ARRAY self->_wishes[...].
        5.  APÓS CONSUMIR TODOS OS PRATOS DESEJADOS, O CLIENTE DEVERÁ SAIR IMEDIATAMENTE DA ESTEIRA.
        6.  QUANTO O RESTAURANTE FECHAR, O CLIENTE DEVERÁ SAIR IMEDIATAMENTE DA ESTEIRA. 
        7.  CASO O CLIENTE ESTEJA COMENDO QUANDO O SUSHI SHOP FECHAR, ELE DEVE TERMINAR DE COMER E EM SEGUIDA
            SAIR IMEDIATAMENTE DA ESTEIRA.
        8.  LEMBRE-SE DE TOMAR CUIDADO COM ERROS DE CONCORRÊNCIA!
    */ 


    customer_t* self = (customer_t*) arg;
    conveyor_belt_t* conveyor = globals_get_conveyor_belt();

    /* INSIRA SUA LÓGICA AQUI */

    //Bloqueia cada cliente que entra na fila - evita espera ocupada
    pthread_mutex_lock(&self->_customer_mutex);
    if (!globals_get_opened()){
        pthread_exit(NULL);
    }

    // Cliente estrou no seats e calcula seus wishes
    int total_wishes = 0;
    for(int i = 0; i < sizeof(self->_wishes) / sizeof(int); i++)
        total_wishes += self->_wishes[i];

    int max_position = 0;
    int min_position = 1;
    if (self->_seat_position == conveyor->_size - 1) max_position = self->_seat_position;
    else max_position = self->_seat_position + 1;
    if (self->_seat_position - 1 == 0) min_position = 1;
    else min_position = self->_seat_position - 1;

    while(total_wishes > 0 && globals_get_opened()) {

        int food = -1;

        for(int i = min_position; i <= max_position; i++) {
            pthread_mutex_lock(&conveyor->_individual_food_slots[i]);
            
            // Caso em que não há nada na posição i -> desocupa a posição e vai para a próxima
            if (conveyor->_food_slots[i] == -1) {
                pthread_mutex_unlock(&conveyor->_individual_food_slots[i]);
                continue;
            }
            // Caso em que o que está na posição é desejado -> realiza o pick food
            if (self->_wishes[conveyor->_food_slots[i]] > 0) {
                food = conveyor -> _food_slots[i];
                customer_pick_food(i);
                total_wishes--;
                pthread_mutex_unlock(&conveyor->_individual_food_slots[i]);
                break;
            }
            pthread_mutex_unlock(&conveyor->_individual_food_slots[i]);
        }
        if (food > -1) customer_eat(self, food);
    }
    customer_leave(self);
    customer_finalize(self);
    pthread_exit(NULL);
}

void customer_pick_food(int food_slot) {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  O CLIENTE SÓ PODE COMEÇAR A PEGAR COMIDA APÓS ESTAR SENTADO EM UMA VAGA DA ESTEIRA.
        2.  O CLIENTE SÓ SENTARÁ QUANDO O HOSTESS ATUALIZAR O VALOR customer_t->_seat_position.
        3.  SE VOCÊ AINDA NÃO IMPLEMENTOU O HOSTESS, COMECE POR ELE (VEJA O ARQUIVO `hostess.c`)!
        4.  O CLIENTE PODERÁ PEGAR COMIDA DE TRÊS POSSÍVEIS SLOTS: {i-1, i, i+1}, ONDE i É O ÍNDICE 
            POSICIONAL DO CLIENTE NA ESTEIRA (O ASSENTO ONDE ELE ESTÁ SENTADO).
        5.  NOTE QUE CLIENTES ADJACENTES DISPUTARÃO OS MESMOS PRATOS. CUIDADO COM PROBLEMAS DE SINCRONIZAÇÃO!
    */
   
    /* INSIRA SUA LÓGICA AQUI */
    conveyor_belt_t* conveyor = globals_get_conveyor_belt();
    
    // Lugar da esteira que o cliente pegou a comida fica vazio
    // Mutex da posição já lockado em run
    conveyor-> _food_slots[food_slot] = -1;
}

void customer_eat(customer_t* self, enum menu_item food) {
    /*
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  ESSA FUNÇÃO JÁ VEM COM PARTE DO CÓDIGO PRONTA (OS SLEEPS PARA CADA TIPO DE ALIMENTO).
        2.  LEMBRE-SE DE DECREMENTAR OS ITENS DA LISTA DE DESEJOS DO CLIENTE CONFORME ELE CONSUMIR OS PRATOS.
        3.  A LISTA DE DESEJOS DO CLIENTE É UM ARRAY COM AS QUANTIDADES DESEJADAS DE CADA PRATO.
        4.  CADA PRATO DO MENU (VER ENUM `menu_item` NO ARQUIVO `menu.h` É REPRESENTADO POR UM INTEIRO),
            ENTÃO UM self->_wishes = [0,0,1,2,0] CONDIZ COM O DESEJO DE COMER 1 RAMÉN E 2 ONIGUIRIS.
    */

    dishes_info_t* dishes_info = globals_get_dishes_info();
    pthread_mutex_t* consumed_dishes_mutexes = globals_get_consumed_dishes_mutexes();

    /* INSIRA SUA LÓGICA AQUI */
    // Tira a comida da lista de desejos do cliente
    self->_wishes[food]--;

    //Incrementa variável global - mutex para exclusão mútua
    pthread_mutex_lock(&consumed_dishes_mutexes[food]);
    dishes_info -> consumed_dishes[food]++;
    pthread_mutex_unlock(&consumed_dishes_mutexes[food]);

    /* NÃO EDITE O CONTEÚDO ABAIXO */
    virtual_clock_t* global_clock = globals_get_virtual_clock();
    switch (food) {
        case Sushi:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Sushi!\n", self->_id);
            msleep(SUSHI_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Sushi!\n", self->_id);
            break;
        case Dango:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Dango!\n", self->_id);
            msleep(DANGO_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Dango!\n", self->_id);
            break;
        case Ramen:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Ramen!\n", self->_id);
            msleep(RAMEN_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Ramen!\n", self->_id);
            break;
        case Onigiri:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Onigiri!\n", self->_id);
            msleep(ONIGIRI_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Onigiri!\n", self->_id);
            break;
        case Tofu:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Tofu!\n", self->_id);
            msleep(TOFU_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Tofu!\n", self->_id);
            break; 
        default:
            fprintf(stdout, RED "[ERROR] Invalid menu_item variant passed to `customer_eat()`.\n" NO_COLOR);
            exit(EXIT_FAILURE);
    }
}

void customer_leave(customer_t* self) {
    /*
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  ESSA FUNÇÃO DEVERÁ REMOVER O CLIENTE DO ASSENTO DO CONVEYOR_BELT GLOBAL QUANDO EXECUTADA.
    */
    conveyor_belt_t* conveyor_belt = globals_get_conveyor_belt();

    /* INSIRA SUA LÓGICA AQUI */
    if (self->_seat_position != -1) {
        pthread_mutex_lock(&conveyor_belt->_seats_mutex);
        conveyor_belt->_seats[self->_seat_position] = -1;

        //incrementa no semáforo de assentos vazios
        sem_post(&conveyor_belt->_free_seats_sem);
        if (globals_get_opened())
            globals_set_served_customers(globals_get_served_customers() + 1);
        pthread_mutex_unlock(&conveyor_belt->_seats_mutex);
    }

    fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d left the shop!\n", self->_id);
}

customer_t* customer_init() {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    customer_t* self = malloc(sizeof(customer_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `customer_t* customer_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    self->_id = rand() % 1000;
    for (int i=0; i<=4; i++) {
        self->_wishes[i] = (rand() % 4);
    }
    self->_seat_position = -1;

    // Mutex para cada cliente, que se inicia trancado. 
    // O Hostess que libera os clientes para começarem executar suas funções
    pthread_mutex_init(&self->_customer_mutex, NULL);
    pthread_mutex_lock(&self->_customer_mutex);

    pthread_create(&self->thread, NULL, customer_run, (void *) self);
    return self;
}

void customer_finalize(customer_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    // Libera o mutex de cada cliente que executou suas funções para finalização
    pthread_mutex_unlock(&self->_customer_mutex);

    pthread_join(self->thread, NULL);
    pthread_mutex_destroy(&self->_customer_mutex);
    fprintf(stdout, CYAN "[FREE]" NO_COLOR " Customer %d finalized!\n", self->_id);
    free(self);
}

void print_customer(customer_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    print_virtual_time(globals_get_virtual_clock());
    fprintf(stdout, BROWN "[DEBUG] Customer " NO_COLOR "{\n");
    fprintf(stdout, BROWN "    _id" NO_COLOR ": %d\n", self->_id);
    fprintf(stdout, BROWN "    _wishes" NO_COLOR ": [%d, %d, %d, %d, %d]\n", self->_wishes[0], self->_wishes[1], self->_wishes[2], self->_wishes[3], self->_wishes[4]);
    fprintf(stdout, BROWN "    _seat_position" NO_COLOR ": %d\n", self->_seat_position);
    fprintf(stdout, NO_COLOR "}\n" NO_COLOR);
}
