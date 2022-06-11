
#include "../../include/botonera/botonera_helper.h"

int desconectar_cliente(int sockett) {
    uint32_t cliente_desconect = (uint32_t) OPCODE_CLIENTE_DESCONECTADO;
    send(sockett, &cliente_desconect, sizeof(uint32_t), 0);
}

pthread_t crear_server_botonera_memoria(){
    pthread_t hilo_server_memoria;
    pthread_create(&hilo_server_memoria, NULL, (void *)system,
                   (void *)"../../memoria_swap/bin/memoria_swap.out ../../memoria_swap/arch_config/memoria.config");

    pthread_detach(hilo_server_memoria);

    //cerrar enviando como opcode __ABORT__
    return hilo_server_memoria;
}

void cerrar_server_botonera_memoria(int sockett){
    warning_log("botonera_helper.c@cerrar_server_botonera_memoria", "ENVIO EL ABORT A MEMORIA");
    send_codigo_op(sockett, __ABORT__);
}

uint32_t hacer_acceso_1er_y_2do_nivel(int sockett, uint32_t pid, uint32_t valor_tabla_1er_nivel, uint32_t entrada){

    format_warning_log("botonera_helper.c@hacer_acceso_1er_y_2do_nivel", "valor_tabla_1er_nivel: %i", valor_tabla_1er_nivel);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid, valor_tabla_1er_nivel, entrada);

    uint32_t codopRTA1;
    recv(sockett, &codopRTA1, sizeof(uint32_t), false);

    uint32_t PIDRTA1;
    recv(sockett, &PIDRTA1, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID1;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID1, sizeof(uint32_t), false);


    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid, POSICION_TABLA_2DO_NIVEL_PID1, 0);

    uint32_t opcodee1;
    recv(sockett, &opcodee1, sizeof(uint32_t), false);
    uint32_t pidd1;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco1;
    recv(sockett, &marco1, sizeof(uint32_t), false);


    return marco1;
}
