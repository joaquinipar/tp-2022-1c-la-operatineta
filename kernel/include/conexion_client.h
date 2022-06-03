#ifndef KERNEL_INCLUDE_CONEXION_CLIENT_H_
#define KERNEL_INCLUDE_CONEXION_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include "config_kernel.h"
#include <utils/structs.h>
#include <utils/serializacion.h>


int socket_cliente_kernel;  /*es el que se conecta con memoria*/
int socket_cliente_cpu_dispatch; 
int socket_cliente_cpu_interrupt; 

int iniciar_conexion_memoria();
int iniciar_conexion_cpu_dispatch(); 
int iniciar_conexion_cpu_interrupt(); 
bool enviar_mensaje_valor_tabla_1er_nivel(uint32_t pid, uint32_t tamanio, uint32_t** valor_tabla_1er_nivel); 
bool enviar_mensaje_exit(pcb_t* proceso); 
bool enviar_mensaje_ejecutar(pcb_t* proceso); 
bool enviar_mensaje_exit(pcb_t* proceso); 
bool enviar_mensaje_desalojar_proceso(pcb_t* proceso); 
bool enviar_mensaje_suspender_proceso(pcb_t* proceso);

#endif /* KERNEL_INCLUDE_CONEXION_CLIENT_H_ */
