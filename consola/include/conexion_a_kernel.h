#ifndef CONSOLA_INCLUDE_CONEXION_A_KERNEL_H
#define CONSOLA_INCLUDE_CONEXION_A_KERNEL_H

#include <commons/collections/list.h>
#include <stdint.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/server.h>
#include "config_consola.h"
#include "serializacion.h"

void enviar_proceso_a_kernel(t_list *lista_de_instrucciones, uint32_t tamanio_proceso); 

#endif /* CONSOLA_INCLUDE_CONEXION_A_KERNEL_H */
