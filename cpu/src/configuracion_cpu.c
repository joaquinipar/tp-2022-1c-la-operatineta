#include "../include/configuracion_cpu.h"

void iniciar_config_cpu(char *direccion) {

  t_config* una_config_cpu = config_create(direccion);

  procesar_archivo_config_cpu(una_config_cpu);

  config_destroy(una_config_cpu);
}

void procesar_archivo_config_cpu(t_config *una_config_cpu) {

  int configElements = config_keys_amount(una_config_cpu);

  if (configElements == 0) {
    exit(EXIT_FAILURE);
  } else {
    cargar_archivo_config_cpu(una_config_cpu);
  }
}

config_cpu_t *crear_estructura_cpu_config() {
  config_cpu_t *config_cpu = malloc(sizeof(config_cpu_t));
  config_cpu->reemplazo_tlb = NULL;
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
  cpu_config->reemplazo_tlb = obtener_string_arch_config(una_config_cpu, "REEMPLAZO_TLB");
 

}

void destruir_estructura_cpu_config() {

  free(cpu_config->ip_memoria );
  free(cpu_config->reemplazo_tlb );
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