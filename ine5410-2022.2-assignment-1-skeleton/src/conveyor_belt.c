#include <stdio.h>
#include <stdlib.h>

#include "conveyor_belt.h"
#include "virtual_clock.h"
#include "globals.h"
#include "menu.h"


void* conveyor_belt_run(void* arg) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    conveyor_belt_t* self = (conveyor_belt_t*) arg;
    virtual_clock_t* virtual_clock = globals_get_virtual_clock();
    while (globals_get_opened_restaurant()) {
        msleep(CONVEYOR_IDLE_PERIOD/virtual_clock->clock_speed_multiplier);
        print_virtual_time(globals_get_virtual_clock());
        fprintf(stdout, GREEN "[INFO]" NO_COLOR " Conveyor belt started moving...\n");
        print_conveyor_belt(self);

        msleep(CONVEYOR_MOVING_PERIOD/virtual_clock->clock_speed_multiplier);

        /*CRIAR SEMAFORO QUE CONTA NUMERO DE CLIENTES*/

        pthread_mutex_lock(&self->_food_slots_mutex);
        /* for do tamanho de clientes sentados -> wait num semáforo iniciado com o número de clientes sentados*/
        int last = self->_food_slots[0];
        for (int i=0; i<self->_size-1; i++) {
            pthread_mutex_lock(&self->_each_food_slots[i]);
            pthread_mutex_lock(&self->_each_food_slots[i+1]);
            self->_food_slots[i] = self->_food_slots[i+1];
            pthread_mutex_unlock(&self->_each_food_slots[i]);
            pthread_mutex_unlock(&self->_each_food_slots[i+1]);
        }
        self->_food_slots[self->_size-1] = last;
        
        print_virtual_time(globals_get_virtual_clock());
        fprintf(stdout, GREEN "[INFO]" NO_COLOR " Conveyor belt finished moving...\n");
        /*for com tamanho de clientes sentados -> post num semáforo iniciado com o número de clientes sentados*/
        
        for(int i = 0; i < globals_get_customers_seat(); i++) {
            sem_post(&self->_customers_sem);
        }
        pthread_mutex_unlock(&self->_food_slots_mutex);

        

        print_conveyor_belt(self);
    }
    pthread_exit(NULL);
}

conveyor_belt_t* conveyor_belt_init(config_t* config) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    conveyor_belt_t* self = malloc(sizeof(conveyor_belt_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `conveyor_belt_t* conveyor_belt_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    self->_size = config->conveyor_belt_capacity;
    self->_seats = (int*) malloc(sizeof(int)* self->_size);
    self->_food_slots = (int*) malloc(sizeof(int)* self->_size);
    self->_each_food_slots = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*self->_size);
    for (int i=0; i<self->_size; i++) {
        self->_food_slots[i] = -1;
        self->_seats[i] = -1;
    }
    pthread_mutex_init(&self->_seats_mutex, NULL);
    pthread_mutex_init(&self->_food_slots_mutex, NULL);
    // Adicionados
    sem_init(&self->_free_seats_sem, 0, self->_size);
    sem_init(&self->_empty_slots_sem, 0, self->_size);
    sem_init(&self->_full_slots_sem, 0, 0);
    sem_init(&self->_customers_sem, 0, 0);
    for (int i = 0; i < self->_size; i++)
        pthread_mutex_init(&(self->_each_food_slots[i]), NULL);
    pthread_create(&self->thread, NULL, conveyor_belt_run, (void *) self);
    print_conveyor_belt(self);
    return self;
}

void conveyor_belt_finalize(conveyor_belt_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    pthread_join(self->thread, NULL);
    pthread_mutex_destroy(&self->_seats_mutex);
    pthread_mutex_destroy(&self->_food_slots_mutex);
    //adicionado
    sem_destroy(&self->_empty_slots_sem);
    sem_destroy(&self->_full_slots_sem);
    sem_destroy(&self->_free_seats_sem);
    sem_destroy(&self->_customers_sem);
    for (int i = 0; i < self->_size; i++)
        pthread_mutex_destroy(&(self->_each_food_slots[i]));
    free(self);
}

void print_conveyor_belt(conveyor_belt_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    print_virtual_time(globals_get_virtual_clock());
    fprintf(stdout, BROWN "[DEBUG] Conveyor Belt " NO_COLOR "{\n");
    fprintf(stdout, BROWN "    _size" NO_COLOR ": %d\n", self->_size);
    int error_flag = FALSE;

    fprintf(stdout, BROWN "    _food_slots" NO_COLOR ": [");
    for (int i=0; i<self->_size; i++) {
        if (i%25 == 0) {
            fprintf(stdout, NO_COLOR "\n        ");
        }
        switch (self->_food_slots[i]) {
            case -1:
                fprintf(stdout, NO_COLOR "%s, ", EMPTY_SLOT);
                break;
            case 0:
                fprintf(stdout, NO_COLOR "%s, ", SUSHI);
                break;
            case 1:
                fprintf(stdout, NO_COLOR "%s, ", DANGO);
                break;
            case 2:
                fprintf(stdout, NO_COLOR "%s, ", RAMEN);
                break;
            case 3:
                fprintf(stdout, NO_COLOR "%s, ", ONIGIRI);
                break;
            case 4:
                fprintf(stdout, NO_COLOR "%s, ", TOFU);
                break;
            default:
                fprintf(stdout, RED "[ERROR] Invalid menu item code in the Conveyor Belt.\n" NO_COLOR);
                exit(EXIT_FAILURE);
        }
    }
    fprintf(stdout, NO_COLOR "\n    ]\n");
    
    fprintf(stdout, BROWN "    _seats" NO_COLOR ": [");
    for (int i=0; i<self->_size; i++) {
        if (i%25 == 0) {
            fprintf(stdout, NO_COLOR "\n        ");
        }
        switch (self->_seats[i]) {
            case -1:
                fprintf(stdout, NO_COLOR "%s, ", EMPTY_SLOT);
                break;
            case 0:
                fprintf(stdout, NO_COLOR "%s, ", SUSHI_CHEF);
                break;
            case 1:
                fprintf(stdout, NO_COLOR "%s, ", CUSTOMER);
                break;
            default:
                fprintf(stdout, RED "[ERROR] Invalid seat state code in the Conveyor Belt.\n" NO_COLOR);
                exit(EXIT_FAILURE);
        }
    }
    fprintf(stdout, NO_COLOR "\n    ]\n");
    fprintf(stdout, NO_COLOR "}\n" NO_COLOR);
    //Adicionado
    //printf("CLIENTES_SERVIDOS: %d", globals_get_served_customers());
    fprintf(stdout, GREEN "CLIENTES_SERVIDOS: %d", globals_get_served_customers());
}
