#ifndef KERNEL_INCLUDE_DISPATCHER_H_
#define KERNEL_INCLUDE_DISPATCHER_H_

#include <stdint.h>
#include <stdbool.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include "config_kernel.h"
#include <pthread.h>
#include "structs.h"
#include "conexion_client.h"

int iniciar_conexion_cpu_dispatch();
void enviar_proceso_a_cpu(pcb_t *proceso);

#endif /* KERNEL_INCLUDE_DISPATCHER_H_ */
