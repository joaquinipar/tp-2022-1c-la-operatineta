#ifndef INCLUDE_SERVER_MEM_SWAP_H_
#define INCLUDE_SERVER_MEM_SWAP_H_

#include <utils/logger.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include <signal.h>
#include "configuracion_mem_swap.h"

int socket_server_mem; 

void iniciar_server_mem_swap();  
int escuchar_conexiones_nuevas(int server_socket);
bool procesar_conexion(int cliente_socket);


#endif /*INCLUDE_SERVER_MEM_SWAP_H_*/