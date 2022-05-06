#ifndef MEMORIA_SWAP_INCLUDE_SERVER_MEM_H_
#define MEMORIA_SWAP_INCLUDE_SERVER_MEM_H_

#include <utils/logger.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include <signal.h>
#include "configuracion_mem_swap.h"
#include "memoria_api.h"

int socket_server_mem; 

void iniciar_server_mem_swap(char* ip, char* puerto);
int escuchar_conexiones_nuevas(int server_socket);
bool procesar_conexion(int cliente_socket);
void cerrar_server_memoria(); 
bool enviar_mensaje_inicial_configuracion(int cliente_socket);

#endif /* MEMORIA_SWAP_INCLUDE_SERVER_MEM_H_ */
