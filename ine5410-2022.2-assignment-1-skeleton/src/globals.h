#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "conveyor_belt.h"
#include "virtual_clock.h"
#include "queue.h"

/**
 * @brief Inicializa lista de mutexes de pratos consumidos
 * 
 * @param menu_size: número de pratos do menu
 */
extern void global_consumed_dishes_mutexes_init(int menu_size);

/**
 * @brief Finaliza lista de mutexes de pratos consumidos
 * 
 * @param self: ponteiro para o array de mutexes
 */
extern void global_consumed_dishes_mutexes_finalize(pthread_mutex_t* self);

/**
 * @brief Retorna lista global de mutexes dos pratos consumidos
 */
extern pthread_mutex_t* globals_get_consumed_dishes_mutexes();

/**
 * @brief Inicia um relógio virtual (de modo global)
 * 
 * @param virtual_clock 
 */
extern void globals_set_virtual_clock(virtual_clock_t *virtual_clock);

/**
 * @brief Retorna um relógio virtual (de modo global)
 * 
 * @return virtual_clock_t* 
 */
extern virtual_clock_t *globals_get_virtual_clock();

/**
 * @brief Inicia uma esteira de sushi (de modo global).
 * 
 * @param conveyor_belt
 */
extern void globals_set_conveyor_belt(conveyor_belt_t *conveyor_belt);

/**
 * @brief Retorna uma esteira de sushi (de modo global)
 * 
 * @return conveyor_belt_t* 
 */
extern conveyor_belt_t *globals_get_conveyor_belt();

/**
 * @brief Inicia uma fila (de modo global)
 * 
 * @param queue 
 */
extern void globals_set_queue(queue_t *queue);

/**
 * @brief Retorna uma fila (de modo global)
 * 
 * @return queue_t* 
 */
extern queue_t *globals_get_queue();

/* VARIÁVEIS GLOBAIS CRIADAS*/

/**
 * @brief Inicia contador de clientes servidos (de modo global)
 * 
 * @param 
 */
void globals_set_served_customers();

/**
 * @brief Retorna ponteiro para o contador de cientes servidos
 * 
 * @return int
 */
extern int globals_get_served_customers();

/**
 * @brief Informações sobre pratos preparados e consumidos
*/
typedef struct dishes_info {
    int* prepared_dishes;
    int* consumed_dishes;
} dishes_info_t;

/**
 * @brief Inicia informações sobre pratos preparados e consumidos
 * 
 * @param dishes_info
 */
void globals_set_dishes_info(dishes_info_t *dishes_info);

/**
 * @brief Retorna ponteiro para informações de pratos preparados e consumidos
 * 
 * @return dishes_info_t* 
 */
extern dishes_info_t* globals_get_dishes_info();

/**
 * @brief Cria variável do tipo dishes_info_t com malloc
 * 
 * @return dishes_info_t* 
 */
dishes_info_t* dishes_info_init(int menu_size);

/**
 * @brief Finaliza dishes_info com free()
*/
void dishes_info_finalize(dishes_info_t* self);

/**
 * @brief Retorna variável que indica se o restaurante está aberto
 * 
 * @return dishes_info_t* 
 */
unsigned int globals_get_opened();

/**
 * @brief seta_variável que indica restaurante aberto
 */
void globals_set_opened(int opened);

/**
 * @brief imprime resultados da simulação
 */
void print_simulation_counters(dishes_info_t* info, int served_customers);

/**
 * @brief Finaliza todas as variáveis globais.
 * 
 */
extern void globals_finalize();

#endif  // __GLOBALS_H__
