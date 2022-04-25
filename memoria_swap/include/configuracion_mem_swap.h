#ifndef INCLUDE_CONFIGURACION_MEM_SWAP_H_
#define INCLUDE_CONFIGURACION_MEM_SWAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/error.h>
#include <commons/string.h>
#include <utils/logger.h>
typedef enum algoritmo_t { CLOCK, CLOCKMOD } algoritmo_t;

typedef struct {
   int tam_memoria;
   int tam_pagina;
   int entradas_por_tabla;
   int retardo_memoria;
   int marcos_por_proceso;
   int retardo_swap;
   char* puerto_escucha;
   char* ip_escucha;
   algoritmo_t algoritmo_reeemplazo;
   char* path_swap;
  
}config_mem_t;

config_mem_t* mem_swap_config;
void iniciar_config(int argc, char* argv[]);
void iniciar_config_mem_swap(char *direccion);
void procesar_archivo_config_mem_swap(t_config *una_config_mem_swap);
config_mem_t *crear_estructura_mem_swap_config();
void cargar_archivo_config_mem_swap(t_config *una_config_mem_swap);
void destruir_estructura_mem_swap_config();
void terminar_proceso_configuracion(char *MensajeError);
char *obtener_string_arch_config(t_config *configuracion, char *clave);
int obtener_int_arch_config(t_config *configuracion, char *clave);
algoritmo_t obtener_algoritmo_enum(char * tipo_algoritmo);

#endif /*INCLUDE_CONFIGURACION_MEM_SWAP_H_*/