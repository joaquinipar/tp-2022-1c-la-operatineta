#ifndef MEMORIA_SWAP_BOTONERA_HELPER_H
#define MEMORIA_SWAP_BOTONERA_HELPER_H

#include <stdint.h>
#include <utils/protocolo_comunicacion.h>
#include <stdlib.h>
#include <pthread.h>
#include <utils/logger.h>

pthread_mutex_t sum_mutex;

int desconectar_cliente(int sockett);
pthread_t crear_server_botonera_memoria();
void cerrar_server_botonera_memoria(int sockett);

#endif //MEMORIA_SWAP_BOTONERA_HELPER_H
