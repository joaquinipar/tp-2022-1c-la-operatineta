#include "../include/configuracion_mem_swap.h"

void iniciar_config_mem_swap(char *direccion){

  t_config* una_config_mem_swap = config_create(direccion);

  procesar_archivo_config_mem_swap(una_config_mem_swap);

  config_destroy(una_config_mem_swap);

}
void procesar_archivo_config_mem_swap(t_config* una_config_mem_swap){
  info_log("configuracion_mem_swap.c@procesar_archivo_config_mem_swap", "Se comienza a procesar Archivo de Configuracion de Memoria - Swap ");

  int configElements = config_keys_amount(una_config_mem_swap);

  if (configElements == 0) {
    error_log("configuracion_mem_swap.c@procesar_archivo_config_mem_swap", "No hay claves en el archivo - Archivo de configuración inválido");
    exit(EXIT_FAILURE);
  } else {
    cargar_archivo_config_mem_swap(una_config_mem_swap);
  }

}
config_mem_t *crear_estructura_mem_swap_config(){

  config_mem_t* config_mem_swap = malloc(sizeof(config_mem_t));
  config_mem_swap->path_swap = NULL;
  config_mem_swap->puerto_escucha = NULL;
  config_mem_swap->ip_escucha= NULL;
  return config_mem_swap;
}
void cargar_archivo_config_mem_swap(t_config *una_config_mem_swap){

 mem_swap_config = crear_estructura_mem_swap_config();
 mem_swap_config->entradas_por_tabla = obtener_int_arch_config(una_config_mem_swap, "ENTRADAS_POR_TABLA");
 mem_swap_config->tam_memoria = obtener_int_arch_config(una_config_mem_swap, "TAM_MEMORIA");
 mem_swap_config->tam_pagina =obtener_int_arch_config(una_config_mem_swap, "TAM_PAGINA");
 mem_swap_config->retardo_memoria =obtener_int_arch_config(una_config_mem_swap, "RETARDO_MEMORIA");
 mem_swap_config->marcos_por_proceso =obtener_int_arch_config(una_config_mem_swap, "MARCOS_POR_PROCESO");
 mem_swap_config->retardo_memoria =obtener_int_arch_config(una_config_mem_swap, "RETARDO_SWAP");
 
 mem_swap_config->path_swap =obtener_string_arch_config(una_config_mem_swap, "PATH_SWAP");
 
char* algoritmo = obtener_string_arch_config(una_config_mem_swap, "ALGORITMO_REEMPLAZO");
mem_swap_config->algoritmo_reeemplazo = obtener_algoritmo_enum(algoritmo);
free(algoritmo);

mem_swap_config->ip_escucha=obtener_string_arch_config(una_config_mem_swap, "IP_ESCUCHA");

int puerto_escucha =obtener_int_arch_config(una_config_mem_swap, "PUERTO_ESCUCHA");
char* puerto_esc = string_itoa(puerto_escucha);
mem_swap_config->puerto_escucha = string_duplicate(puerto_esc);
free(puerto_esc); 


}
void destruir_estructura_mem_swap_config(){
    info_log("configuracion_mem_swap.c@destruir_estructura_mem_swap_config", "Se destruye la estructura de configuracion MEMORIA - SWAP");

    free(mem_swap_config->path_swap);
    free(mem_swap_config->puerto_escucha);
   free(mem_swap_config->ip_escucha);
    free(mem_swap_config);
}

void terminar_proceso_configuracion(char *MensajeError) {
  error_show(MensajeError); // Muestra un mensaje de error alertando del problema
  printf("\n");
  exit(EXIT_FAILURE); // Finaliza el proceso
}


char *obtener_string_arch_config(t_config *configuracion, char *clave) {
  char *valor = NULL;
  if (config_has_property(configuracion, clave)) {
    valor = string_duplicate(config_get_string_value(configuracion, clave)); 
    if (valor == NULL)
      terminar_proceso_configuracion("Error: Archivo de configuracion sin "
                                     "valores definidos de Tipo String");
  } else
    terminar_proceso_configuracion("Error: Archivo de configuracion "
                                   "incompleto. Falta clave de tipo string ");

  return valor;
}

int obtener_int_arch_config(t_config *configuracion, char *clave) {
  int valor;
  if (config_has_property(configuracion, clave)) 
  {
    valor = config_get_int_value(configuracion, clave); 

  } else
        terminar_proceso_configuracion("Error: Archivo de configuracion incompleto. Falta clave de tipo Int ");

  return valor;
}

algoritmo_t obtener_algoritmo_enum(char *algoritmo) {
  algoritmo_t algoritmo_reemplazo;

  if (!strcmp(algoritmo, "CLOCK-M")) {
    algoritmo_reemplazo = CLOCKMOD;
    info_log("configuracion_mem_swap.c@obtener_algoritmo_enum", "Algoritmo de reemplazo: CLOCK-MODIFICADO");
    
  } else if (!strcmp(algoritmo, "CLOCK")) {
   info_log("configuracion_mem_swap.c@obtener_algoritmo_enum", "Algoritmo de reemplazo: CLOCK");

    algoritmo_reemplazo = CLOCK;
  } else {
    info_log("configuracion_mem_swap.c@obtener_algoritmo_enum", "Algoritmo de reemplazo no disponible. Se setea por default CLOCK");
    algoritmo_reemplazo = CLOCK;
  }

  return algoritmo_reemplazo;
}