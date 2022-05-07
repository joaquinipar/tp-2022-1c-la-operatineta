#ifndef CPU_INCLUDE_SERVER_DISPATCH_H_
#define CPU_INCLUDE_SERVER_DISPATCH_H_

#include <signal.h>
#include <stdbool.h>
#include <utils/logger.h>
#include <utils/server.h>
#include <pthread.h>
#include <utils/protocolo_comunicacion.h>
#include "configuracion_cpu.h"


int socket_server_dispatch;
int socket_server_interrupt;

pthread_t iniciar_server_dispatch();
int escuchar_conexiones_nuevas_dispatch(int server_socket); 
bool procesar_conexion_dispatch(int cliente_socket);


pthread_t iniciar_server_interrupt();
int escuchar_conexiones_nuevas_interrupt(int server_interrupt_socket); 
bool procesar_conexion_interrupt(int cliente_socket);

#endif /* CPU_INCLUDE_SERVER_DISPATCH_H_ */
