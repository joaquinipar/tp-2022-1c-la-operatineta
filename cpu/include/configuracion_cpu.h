#ifndef INCLUDE_CONFIGURACION_CPU_H_
#define INCLUDE_CONFIGURACION_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/error.h>
#include <commons/string.h>
#include <utils/logger.h>

typedef enum algoritmo_reemplazo_tlb_t { FIFO, LRU } algoritmo_reemplazo_tlb_t;

typedef struct config_cpu_t {
   int entradas_tlb;
   algoritmo_reemplazo_tlb_t reemplazo_tlb;
   int retardo_noop;
   char* ip_memoria;
   int puerto_memoria;
   int puerto_escucha_dispatch;
   int puerto_escucha_interrupt; 
}config_cpu_t;

config_cpu_t* cpu_config;

void iniciar_config_cpu(char *direccion);
void procesar_archivo_config_cpu(t_config *una_config_cpu);
config_cpu_t *crear_estructura_cpu_config();
void cargar_archivo_config_cpu(t_config *una_config_cpu);
void destruir_estructura_cpu_config();
void terminar_proceso_configuracion(char *MensajeError);
char *obtener_string_arch_config(t_config *configuracion, char *clave);
int obtener_int_arch_config(t_config *configuracion, char *clave);
algoritmo_reemplazo_tlb_t obtener_algoritmo_enum(char *algoritmo);



#endif /* INCLUDE_CONFIGURACION_CPU_H_ */