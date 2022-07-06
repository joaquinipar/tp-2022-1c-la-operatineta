#include "../include/configuracion_cpu.h"

void iniciar_config(int argc, char* argv[]){
       if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [CONFIG_PATH]\n", argv[0]);
        //exit(1);
    }
    if(argc == 1){
        // Caso donde no se paso un archivo de config por parametro
        warning_log("main.c", "No se envió la ruta del archivo de configuración por parametro. Apunto al default ./cpu.config");
        iniciar_config_cpu("./cpu.config");
    }
    else{
        // Caso donde le pasamos la ruta del config por parametro
        iniciar_config_cpu(argv[1]);
    }
}

void iniciar_config_cpu(char*direccion) {

  t_config* una_config_cpu = config_create(direccion);

  	if(una_config_cpu == NULL){
	  error_log("configuracion_cpu.c@iniciar_config_cpu", "Ocurrio un error al intentar abrir el Archivo de Configuracion de CPU");
    }

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
  config_cpu->ip_escucha=NULL;
  config_cpu->puerto_memoria=NULL;
  config_cpu->puerto_escucha_dispatch=NULL;
  config_cpu->puerto_escucha_interrupt=NULL;
  return config_cpu;
}

void cargar_archivo_config_cpu(t_config* una_config_cpu) {

  cpu_config = crear_estructura_cpu_config();
  cpu_config->entradas_tlb = obtener_int_arch_config(una_config_cpu, "ENTRADAS_TLB");
  cpu_config->retardo_noop = obtener_int_arch_config(una_config_cpu, "RETARDO_NOOP");

  cpu_config->ip_memoria = obtener_string_arch_config(una_config_cpu, "IP_MEMORIA");
  cpu_config->ip_escucha = obtener_string_arch_config(una_config_cpu, "IP_ESCUCHA");

  char* algoritmo = obtener_string_arch_config(una_config_cpu, "REEMPLAZO_TLB");
  cpu_config->reemplazo_tlb = obtener_algoritmo_enum(algoritmo);
  free(algoritmo);

  int puerto_mem= obtener_int_arch_config(una_config_cpu, "PUERTO_MEMORIA");
  char* puerto_memoria = string_itoa(puerto_mem);
  cpu_config->puerto_memoria = string_duplicate(puerto_memoria);
  free(puerto_memoria); 

  int puerto_disp= obtener_int_arch_config(una_config_cpu, "PUERTO_ESCUCHA_DISPATCH");
  char* puerto_dispatch = string_itoa(puerto_disp);
  cpu_config->puerto_escucha_dispatch = string_duplicate(puerto_dispatch);
  free(puerto_dispatch); 

  int puerto_escucha_interrup= obtener_int_arch_config(una_config_cpu, "PUERTO_ESCUCHA_INTERRUPT");
  char* puerto_interrupt = string_itoa(puerto_escucha_interrup);
  cpu_config->puerto_escucha_interrupt = string_duplicate(puerto_interrupt);
  free(puerto_interrupt); 

}

void destruir_estructura_cpu_config() {
  info_log("configuracion_cpu.c@destruir_estructura_cpu_config", "Se destruye la estructura de configuracion CPU");
  free(cpu_config->ip_memoria);
  free(cpu_config->ip_escucha);
  free(cpu_config->puerto_memoria);
  free(cpu_config->puerto_escucha_dispatch);
  free(cpu_config->puerto_escucha_interrupt);
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
    info_log("configuracion_cpu.c@obtener_algoritmo_enum", "Algoritmo de reemplazo para las entradas de la TLB: FIFO");
    
  } else if (!strcmp(algoritmo, "LRU")) {
   info_log("configuracion_cpu.c@.c@obtener_algoritmo_enum", "Algoritmo de reemplazo para las entradas de la TLB: LRU");

    algoritmo_reemplazo = LRU;
  } else {
    info_log("configuracion_cpu.c@.c@obtener_algoritmo_enum", "Algoritmo de reemplazo para las entradas de la TLB no disponible. Se setea por default FIFO");
    algoritmo_reemplazo = FIFO;
  }

  return algoritmo_reemplazo;
}