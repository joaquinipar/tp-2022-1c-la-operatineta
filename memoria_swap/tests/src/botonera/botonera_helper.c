
#include "../../include/botonera/botonera_helper.h"

int desconectar_cliente(int sockett) {
    uint32_t cliente_desconect = (uint32_t) CLIENTE_DESCONECTADO;
    send(sockett, &cliente_desconect, sizeof(uint32_t), 0);
}

pthread_t crear_server_botonera_memoria(){
    pthread_t hilo_server_memoria;
    pthread_create(&hilo_server_memoria, NULL, (void *)system,
                   (void *)"../../memoria_swap/bin/memoria_swap.out ../../memoria_swap/arch_config/memoria.config");

    pthread_detach(hilo_server_memoria);

    sleep(1);

    //cerrar enviando como opcode __ABORT__
    return hilo_server_memoria;
}

void cerrar_server_botonera_memoria(int sockett){
    send_codigo_op(sockett, __ABORT__);
}