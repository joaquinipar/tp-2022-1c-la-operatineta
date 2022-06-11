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
uint32_t hacer_acceso_1er_y_2do_nivel(int sockett, uint32_t pid, uint32_t valor_tabla_1er_nivel, uint32_t entrada);
#endif //MEMORIA_SWAP_BOTONERA_HELPER_H
