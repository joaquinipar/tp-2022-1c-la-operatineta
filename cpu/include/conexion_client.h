#ifndef CPU_INCLUDE_CONEXION_CLIENT_H_
#define CPU_INCLUDE_CONEXION_CLIENT_H_

#include <utils/logger.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/serializacion.h>
#include <utils/structs.h>
#include "configuracion_cpu.h"

int socket_cliente_cpu;
int iniciar_conexion_memoria();

typedef struct {
uint32_t cantidad_entradas_tabla;
uint32_t tamanio_pagina;
}mensaje_configuracion_t;

mensaje_configuracion_t* datos_traduccion;

uint32_t enviar_mensaje_acceso_1er_nivel(uint32_t pid, uint32_t entrada_1er_nivel, uint32_t nro_tabla_1er_nivel); 
uint32_t enviar_mensaje_acceso_2do_nivel(uint32_t pid, uint32_t nro_tabla_2do_nivel, uint32_t entrada_2do_nivel);
uint32_t enviar_mensaje_read(uint32_t pid, uint32_t direccion_fisica);
uint32_t enviar_mensaje_write(uint32_t pid, uint32_t direccion_fisica, uint32_t valor_a_escribir);

bool enviar_mensaje_inicial_configuracion(); 
bool enviar_mensaje_proceso_desalojado_io(pcb_t* proceso_actualizado, int socket, uint32_t tiempo_bloqueo); 
bool enviar_mensaje_proceso_desalojado_interrupt(pcb_t* proceso_actualizado, int socket); 
bool enviar_mensaje_proceso_desalojado_exit(pcb_t* proceso_actualizado, int socket); 

#endif /* CPU_INCLUDE_CONEXION_CLIENT_H_ */
