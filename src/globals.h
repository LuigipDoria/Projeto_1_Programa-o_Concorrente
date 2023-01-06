#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "conveyor_belt.h"
#include "virtual_clock.h"
#include "queue.h"
#include "menu.h"

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





// PARTE NÃO ORIGINAL

/**
 * @brief Incrementa a quantidade de clientes que sentaram
 * 
 */
extern void globals_set_num_clientes();

/**
 * @brief Retorna a quantidade de clientes que sentaram
 * 
 */
extern int globals_get_num_clientes();

/**
 * @brief Incrementa a quantidade produzida de cada alimento
 * 
 * @param menu_item
 */
extern void globals_set_num_prod(enum menu_item menu_item);

/**
 * @brief Retorna a quantidade produzida de um alimento
 * 
 * @return menu_item* 
 */
extern int globals_get_num_prod(enum menu_item menu_item);

/**
 * @brief Incrementa a quantidade consumida de cada alimento
 * 
 * @param menu_item
 */
extern void globals_set_num_cons(enum menu_item menu_item);

/**
 * @brief Retorna a quantidade consumida de um alimento
 * 
 * @return menu_item* 
 */
extern int globals_get_num_cons(enum menu_item menu_item);

/**
 * @brief Muda o estado do restaurante para fechado
 * 
 */
extern void globals_set_estado_restaurante();

/**
 * @brief Retorna o estado do restaurante
 * 
 */
extern int globals_get_estado_restaurante();

/**
 * @brief Muda o estado do restaurante para fechado
 * 
 */
extern void globals_set_n_codigos_finalizados();

/**
 * @brief Retorna o estado do restaurante
 * 
 */
extern int globals_get_n_codigos_finalizados();

// FIM PARTE NÃO ORIGINAL





/**
 * @brief Finaliza todas as variáveis globais.
 * 
 */
extern void globals_finalize();

#endif  // __GLOBALS_H__
