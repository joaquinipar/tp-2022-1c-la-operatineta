#ifndef KERNEL_INCLUDE_SERVER_KERNEL_H_
#define KERNEL_INCLUDE_SERVER_KERNEL_H_

#include <pthread.h>
#include <signal.h>

#include <utils/server.h>
#include <utils/logger.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/serializacion.h>
#include "config_kernel.h"
#include "kernel_api.h"


pthread_t iniciar_server_kernel(); 
int escuchar_conexiones_nuevas(int server_socket); 
bool recibir_mensaje_proceso_nuevo(int cliente_socket); 
bool procesar_conexion(int cliente_socket);

#endif /* KERNEL_INCLUDE_SERVER_KERNEL_H_ */
