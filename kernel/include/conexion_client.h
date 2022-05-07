#ifndef KERNEL_INCLUDE_CONEXION_CLIENT_H_
#define KERNEL_INCLUDE_CONEXION_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include "config_kernel.h"


int socket_cliente_kernel;
int socket_cliente_cpu_dispatch; 
int socket_cliente_cpu_interrupt; 
int iniciar_conexion_memoria();

#endif /* KERNEL_INCLUDE_CONEXION_CLIENT_H_ */
