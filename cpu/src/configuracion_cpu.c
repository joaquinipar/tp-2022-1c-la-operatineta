#include "../include/configuracion_cpu.h"

void iniciar_config_cpu(char *direccion) {

  t_config* una_config_cpu = config_create(direccion);

  procesar_archivo_config_cpu(una_config_cpu);

  config_destroy(una_config_cpu);
}

void procesar_archivo_config_cpu(t_config *una_config_cpu) {
  info_log("configuracion_cpu.c@procesar_archivo_config_cpu", "Se comienza a procesar Archivo de Configuracion de CPU ");

  int configElements = config_keys_amount(una_config_cpu);

  if (configElements == 0) {
    error_log("configuracion_cpu.c@procesar_archivo_config_cpu", "No hay claves en el archivo - Archivo de configuración inválido");
    exit(EXIT_FAILURE);
  } else {
    cargar_archivo_config_cpu(una_config_cpu);
  }
}

config_cpu_t *crear_estructura_cpu_config() {
  config_cpu_t *config_cpu = malloc(sizeof(config_cpu_t));
  config_cpu->ip_memoria = NULL;
  return config_cpu;
}

void cargar_archivo_config_cpu(t_config* una_config_cpu) {

  cpu_config = crear_estructura_cpu_config();
  cpu_config->entradas_tlb = obtener_int_arch_config(una_config_cpu, "ENTRADAS_TLB");
  cpu_config->retardo_noop = obtener_int_arch_config(una_config_cpu, "RETARDO_NOOP");
  cpu_config->puerto_memoria = obtener_int_arch_config(una_config_cpu, "PUERTO_MEMORIA");
  cpu_config->puerto_escucha_dispatch = obtener_int_arch_config(una_config_cpu,"PUERTO_ESCUCHA_DISPATCH");
  cpu_config->puerto_escucha_interrupt = obtener_int_arch_config(una_config_cpu,"PUERTO_ESCUCHA_INTERRUPT");
  cpu_config->ip_memoria = obtener_string_arch_config(una_config_cpu, "IP_MEMORIA");

  char* algoritmo = obtener_string_arch_config(una_config_cpu, "REEMPLAZO_TLB");
  cpu_config->reemplazo_tlb = obtener_algoritmo_enum(algoritmo);
  free(algoritmo);

}

void destruir_estructura_cpu_config() {
  info_log("configuracion_cpu.c@destruir_estructura_cpu_config", "Se destruye la estructura de configuracion CPU");
  free(cpu_config->ip_memoria );
  free(cpu_config); 

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

algoritmo_reemplazo_tlb_t obtener_algoritmo_enum(char *algoritmo) {
  algoritmo_reemplazo_tlb_t algoritmo_reemplazo;

  if (!strcmp(algoritmo, "FIFO")) {
    algoritmo_reemplazo = FIFO;
    info_log("configuracion_cpu.c@obtener_algoritmo_enum", "Algoritmo de reemplazo: FIFO");
    
  } else if (!strcmp(algoritmo, "LRU")) {
   info_log("configuracion_cpu.c@.c@obtener_algoritmo_enum", "Algoritmo de reemplazo: LRU");

    algoritmo_reemplazo = LRU;
  } else {
    info_log("configuracion_cpu.c@.c@obtener_algoritmo_enum", "Algoritmo de reemplazo no disponible. Se setea por default FIFO");
    algoritmo_reemplazo = FIFO;
  }

  return algoritmo_reemplazo;
}