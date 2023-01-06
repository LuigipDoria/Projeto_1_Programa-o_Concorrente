#include <stdlib.h>

#include "globals.h"

/*
    VOCÊ DEVE CRIAR VARIÁVEIS GLOBAIS PARA ARMAZENAR DADOS SOBRE A SIMULAÇÃO.
    NOTAS:
    1.  OS SEGUINTES DADOS DEVEM SER ARMAZENADOS E PRINTADOS AO FIM DA SIMULAÇÃO:
        a. Quantidade de clientes que sentaram-se no conveyor e consumiram todos os itens desejados   -> FEITO
        b. Quantidades produzidas de cada alimento  -> FEITO
        c. Quantidades consumidas de cada alimento  -> FEITO
    2.  SIGA OS EXEMPLOS DE VARIÁVEIS GLOBAIS JÁ EXISTENTES NESSE ARQUIVO PARA CRIAR AS NOVAS.
*/

virtual_clock_t* global_virtual_clock = NULL;
conveyor_belt_t* global_conveyor_belt = NULL;
queue_t* global_queue = NULL;

// PARTE NÃO ORIGINAL

int n_clientes = 0; // Indica a quantidade de clientes que consumiram todos os itens desejados

int n_sushi_prod   = 0; // Indica a quantidade de sushi que foi produzida
int n_dango_prod   = 0; // Indica a quantidade de dango que foi produzida
int n_ramen_prod   = 0; // Indica a quantidade de ramen que foi produzida
int n_onigiri_prod = 0; // Indica a quantidade de onigiri que foi produzida
int n_tofu_prod    = 0; // Indica a quantidade de tofu que foi produzida

int n_sushi_cons   = 0; // Indica a quantidade de sushi que foi consumida
int n_dango_cons   = 0; // Indica a quantidade de dango que foi consumida
int n_ramen_cons   = 0; // Indica a quantidade de ramen que foi consumida
int n_onigiri_cons = 0; // Indica a quantidade de onigiri que foi consumida
int n_tofu_cons    = 0; // Indica a quantidade de tofu que foi consumida

int estado_restaurante = 0; // aberto = 0 fechado = 1

int n_codigos_finalizados = 0;

// FIM PARTE NÃO ORIGINAL

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





// PARTE NÃO ORIGINAL

/*
    Além das variaveis que foram solicitadas, foi criada mais uma que indica o estado de funcionamento do sushi shop
    Para não ser criada uma função pra cada prato foi usado switch case pra simplicar o codigo.
*/

void globals_set_num_clientes() {
    n_clientes = n_clientes + 1;
}

int globals_get_num_clientes() {
    return n_clientes;
}

void globals_set_num_prod(enum menu_item menu_item) {
    switch (menu_item) {
        case Sushi:
            n_sushi_prod = n_sushi_prod + 1;
            break;

        case Dango:
            n_dango_prod = n_dango_prod + 1;
            break;

        case Ramen:
            n_ramen_prod = n_ramen_prod + 1;
            break;

        case Onigiri:
            n_onigiri_prod = n_onigiri_prod + 1;
            break;

        case Tofu:
            n_tofu_prod = n_tofu_prod+ 1;
            break;

        default:
            fprintf(stdout, RED "[ERROR] Invalid menu_item variant passed to `globals_set_num_prod()`.\n" NO_COLOR);
            exit(EXIT_FAILURE);
            break;
    }
}

int globals_get_num_prod(enum menu_item menu_item) {
    switch (menu_item) {
        case Sushi:
            return n_sushi_prod;
            break;

        case Dango:
            return n_dango_prod;
            break;

        case Ramen:
            return n_ramen_prod;
            break;

        case Onigiri:
            return n_onigiri_prod;
            break;

        case Tofu:
            return n_tofu_prod;
            break;

        default:
            fprintf(stdout, RED "[ERROR] Invalid menu_item variant passed to `globals_get_num_prod()`.\n" NO_COLOR);
            exit(EXIT_FAILURE);
            break;
    }
}

void globals_set_num_cons(enum menu_item menu_item) {
    switch (menu_item) {
        case Sushi:
            n_sushi_cons = n_sushi_cons + 1;
            break;

        case Dango:
            n_dango_cons = n_dango_cons + 1;
            break;

        case Ramen:
            n_ramen_cons = n_ramen_cons + 1;
            break;

        case Onigiri:
            n_onigiri_cons = n_onigiri_cons + 1;
            break;

        case Tofu:
            n_tofu_cons = n_tofu_cons + 1;
            break;

        default:
            fprintf(stdout, RED "[ERROR] Invalid menu_item variant passed to `globals_set_num_cons()`.\n" NO_COLOR);
            exit(EXIT_FAILURE);
            break;
    }
}

int globals_get_num_cons(enum menu_item menu_item) {
    switch (menu_item) {
        case Sushi:
            return n_sushi_cons;
            break;

        case Dango:
            return n_dango_cons;
            break;

        case Ramen:
            return n_ramen_cons;
            break;

        case Onigiri:
            return n_onigiri_cons;
            break;

        case Tofu:
            return n_tofu_cons;
            break;

        default:
            fprintf(stdout, RED "[ERROR] Invalid menu_item variant passed to `globals_get_num_cons()`.\n" NO_COLOR);
            exit(EXIT_FAILURE);
            break;
    }
}

void globals_set_estado_restaurante() {
    estado_restaurante = 1;
}

int globals_get_estado_restaurante() {
    return estado_restaurante;
}

void globals_set_n_codigos_finalizados() {
    n_codigos_finalizados++;
}

int globals_get_n_codigos_finalizados() {
    return n_codigos_finalizados;
}


// FIM PARTE NÃO ORIGINAL

/**
 * @brief Finaliza todas as variáveis globais.
 * Se criar alguma variável global que faça uso de mallocs, lembre-se sempre 
 * de usar o free dentro dessa função.
 */
void globals_finalize() {
    virtual_clock_finalize(global_virtual_clock);
    conveyor_belt_finalize(global_conveyor_belt);
}
