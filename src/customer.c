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
        2.  APÓS SENTAR-SE, O CLIENTE COMEÇARÁ PEGAR E COMER OS espera_comidaS DA ESTEIRA.
        3.  O CLIENTE SÓ PODERÁ PEGAR UM espera_comida QUANDO A ESTEIRA ESTIVER PARADA.
        4.  O CLIENTE SÓ PEGARÁ espera_comidaS CASO ELE DESEJE-OS, INFORMAÇÃO CONTIDA NO ARRAY self->_wishes[...].
        5.  APÓS CONSUMIR TODOS OS espera_comidaS DESEJADOS, O CLIENTE DEVERÁ SAIR IMEDIATAMENTE DA ESTEIRA.
        6.  QUANTO O RESTAURANTE FECHAR, O CLIENTE DEVERÁ SAIR IMEDIATAMENTE DA ESTEIRA. 
        7.  CASO O CLIENTE ESTEJA COMENDO QUANDO O SUSHI SHOP FECHAR, ELE DEVE TERMINAR DE COMER E EM SEGUIDA
            SAIR IMEDIATAMENTE DA ESTEIRA.
        8.  LEMBRE-SE DE TOMAR CUIDADO COM ERROS DE CONCORRÊNCIA!
    */ 
    customer_t* self = (customer_t*) arg;


    /* INSIRA SUA LÓGICA AQUI */
    
    // PARTE NÃO ORIGINAL

    /*
        Essa função foi dividida em 3 whiles:
        
        1. O primeiro while é para quando o customer esteja na fila de espera, caso feche durante esse periodo
        acaba a thread

        2. Após o customer ser colocado em um lugar pelo hostess, ele fica esperando um prato que deseja.
        Quando ele acha um prato ele pega e come, incrementando 1 a variavel global da quantidade desse tipo
        de prato que ja foi consumida. Caso o cliente já tenha comido a quantidade que queria de cada prato
        é incrementado 1 na variavel global de clientes que comeram tudo que queria e ele sai do sushi shop
        acabando com a thread. Caso o sushi shop feche e o customer não tenha comido tudo que queria, ele sai
        do sushi shop sem incrementar 1 na variavel global de clientes que comeram tudo que queria.


        3. Ele fica vendo se o prato na sua esquerda, frente ou direita é um dos que ele deseja comer. Caso ele
        ache ele sai desse while informando a posição do prato que ele quer e o tipo do prato. Caso contrario,
        ele espera a esteira se mover e verifica se dessa fez tem um prato que ele queira. Fica nesse loop ate
        ele achar um prato ou o sushi shop fechar.
    */

    int food_slot = 0; // Posição da comida
    int food = 0; // Tipo da comida
    int espera_comida = 1; // Está esperando um espera_comida chegar = 1 || Pegou o espera_comida que queria = 0
    int terminou = 1; // Sim = 0 || Não = 1
    int aux_terminou = 0; // Quantidade de tipos de espera_comidas que ja comeu quantas vezes queria 

    conveyor_belt_t* conveyor = globals_get_conveyor_belt(); // Recebe a esteira
    virtual_clock_t* virtual_clock = globals_get_virtual_clock(); // Recebe o relogio virtual
    queue_t* queue = globals_get_queue();
    
    while (self->_seat_position == -1) { // Cliente está esperando na fila
        if (globals_get_estado_restaurante() == TRUE) { // Ve se o restaurante fechou
            free(self);
            pthread_exit(NULL); // Caso já tenha fechado, acaba a thread do custumer
        }
    }
    
    while (terminou) { // Fica esperando o cliente pegar todos os espera_comidas que deseja
        
        aux_terminou = 0; // Reseta a contagem, para ver se após ele pegar mais um prato ele vai terminar de comer tudo que queria

        espera_comida = 0; // Reinicia a variavel
        while(espera_comida == 0) {
            pthread_mutex_lock(&conveyor->_food_slots_mutex); // Trava o mutex para que nenhuma outra thread altere e leia o conteudo da lista
            for (int i = self->_seat_position-1; i<=self->_seat_position +1 && i<conveyor->_size; i++) { // Percore as posições que o cliente pode pegar um prato
                for (int j = 0; j<5; j++) { // Percore a lista com a quantidade que o cliente quer de cada prato
                    if (conveyor->_food_slots[i] == j && self->_wishes[j] > 0) { // Caso seja um prato que o customer queria
                        food_slot = i; // É armazenado a posição do prato
                        food = j; // E o tipo do prato
                        espera_comida = 1; // E indica que ele achou um prato que queria
                        break; // Sai do loop dos desejos
                    }
                }
                if (espera_comida) { // Se o customer achou um prato
                    break; // Sai do loop das posições que ele podia pegar um prato
                }
            }
            if (espera_comida == 0) { // Caso o cliente não tenha achado um prato que queria
                pthread_mutex_unlock(&conveyor->_food_slots_mutex); // Destrava o mutex para que outras thread possam ler e alterar a lista
                msleep(CONVEYOR_IDLE_PERIOD/virtual_clock->clock_speed_multiplier); // espera a esteira se mover, para ver se aparece um prato que o cliente queira
                if (globals_get_estado_restaurante() == TRUE) { // Ve se o restaurante fechou, enquanto a esteria se movia
                    break; // Caso já tenha fechado, sai do restaurante
                }
            }
        }

        if (globals_get_estado_restaurante() == TRUE) { // Ve se o restaurante fechou
            pthread_mutex_unlock(&conveyor->_food_slots_mutex); // Destrava o mutex para não gerar nenhum problema
            break; // Sai do restaurante
        }


        globals_get_num_cons(food); // Icrementa a quatidade desse espera_comida que foi consumida
        customer_pick_food(food_slot); // Pega o espera_comida
        customer_eat(self, food); // Come o espera_comida
        

        for (int i = 0; i<5; i++) { // Percore a lista com a quantidade de pratos que o cliente ainda quer
            if (self->_wishes[i] == 0) { // Ve se o cliente já comeu a quantidade que queria de cada espera_comida
                aux_terminou ++; // Caso ja tenha comido a quantidade que ele queria daquele tipo de prato, incrementa a variavel.
            }
        }
        
        if (aux_terminou == 5) { // Ve se o cliente ja comeu tudo que queria
            terminou = 0; // Sai do loop
        }

    }
    
    if (terminou == 0) { // Ve se o cliente comeu tudo que queria
        globals_set_num_clientes(); // Aumenta a quantidade dos clientes que consumiram tudo e foram embora
    }
    
    
    customer_leave(self); // O cliente sai do sushi shop

    // FIM PARTE NÃO ORIGINAL

    //msleep(1000000);  // REMOVA ESTE SLEEP APÓS IMPLEMENTAR SUA SOLUÇÃO!
    free(self);
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
        5.  NOTE QUE CLIENTES ADJACENTES DISPUTARÃO OS MESMOS espera_comidaS. CUIDADO COM PROBLEMAS DE SINCRONIZAÇÃO!
    */

    /* INSIRA SUA LÓGICA AQUI */

    // PARTE NÃO ORIGINAL

    /*
        Essa função apenas retira o prato da esteira. A verificação se é um prato valido ou não é feita no run
        Nessa função so tem apenas o unlock do mutex pois o lock e feito no começo da procura do prato, que está
        na função run.

        Foi feito dessa forma, pois essa função é void nao podendo retornar uma flag que indicaria se foi encontrado
    */

    conveyor_belt_t* conveyor = globals_get_conveyor_belt(); // Recebe a esteira
    
    conveyor->_food_slots[food_slot] = -1; // Tira o espera_comida da esteira        
    
    pthread_mutex_unlock(&conveyor->_food_slots_mutex); // Destrava o mutex para que outras thread possam ler e alterar a lista

    // FIM PARTE NÃO ORIGINAL
}

void customer_eat(customer_t* self, enum menu_item food) {
    /*
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  ESSA FUNÇÃO JÁ VEM COM PARTE DO CÓDIGO PRONTA (OS SLEEPS PARA CADA TIPO DE ALIMENTO).
        2.  LEMBRE-SE DE DECREMENTAR OS ITENS DA LISTA DE DESEJOS DO CLIENTE CONFORME ELE CONSUMIR OS espera_comidaS.
        3.  A LISTA DE DESEJOS DO CLIENTE É UM ARRAY COM AS QUANTIDADES DESEJADAS DE CADA espera_comida.
        4.  CADA espera_comida DO MENU (VER ENUM `menu_item` NO ARQUIVO `menu.h` É REPRESENTADO POR UM INTEIRO),
            ENTÃO UM self->_wishes = [0,0,1,2,0] CONDIZ COM O DESEJO DE COMER 1 RAMÉN E 2 ONIGUIRIS.
    */

    /* INSIRA SUA LÓGICA AQUI */

    // PARTE NÃO ORIGINAL

    /*
        Essa função so faz alterações no proprio cliente, dessa forma não precisa de mutex para garantir o funicionamento
        corrento dessa função.
    */

    self->_wishes[food]--; // Diminui a quantidade de um espera_comida que o consumidor quer comer

    // FIM PARTE NÃO ORIGINAL

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

    // PARTE NÃO ORIGINAL

    globals_set_num_cons(food); // Aumenta a quantidade desse espera_comida que foi consumida

    // FIM DA PARTE NÃO ORIGINAL
}

void customer_leave(customer_t* self) {
    /*
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  ESSA FUNÇÃO DEVERÁ REMOVER O CLIENTE DO ASSENTO DO CONVEYOR_BELT GLOBAL QUANDO EXECUTADA.
    */
    conveyor_belt_t* conveyor_belt = globals_get_conveyor_belt();

    /* INSIRA SUA LÓGICA AQUI */

    // PARTE NÃO ORIGINAL

    /*
        Foi colocado um mutex para garantir que nenhuma thread leia a lista durante a saida de um consumidor
    */

    pthread_mutex_lock(&conveyor_belt->_food_slots_mutex); // Trava o mutex para mudar a lista
    conveyor_belt->_seats[self->_seat_position] = -1; // Libera uma vaga no sushi shop
    pthread_mutex_unlock(&conveyor_belt->_food_slots_mutex); // Destrava o mutex para mudar a lista

    // FIM PARTE NÃO ORIGINAL

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
    pthread_create(&self->thread, NULL, customer_run, (void *) self);

    return self;
}

void customer_finalize(customer_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */

    pthread_join(self->thread, NULL);

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
