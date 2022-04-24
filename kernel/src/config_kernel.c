#include "config_kernel.h"

void iniciar_config(int argc, char* argv[]){
       if (argc < 2) {
        fprintf(stderr, "Se esperaba: %s [CONFIG_PATH]\n", argv[0]);
    }
    if(argc == 1){
        // Caso donde no se paso un archivo de config por parametro
        warning_log("main.c", "No se envió la ruta del archivo de configuración por parametro. Apunto al default ./kernel.config");
        iniciar_config_kernel("./kernel.config");
    }
    else{
        // Caso donde le pasamos la ruta del config por parametro
        iniciar_config_kernel(argv[1]);
    }
}

void iniciar_config_kernel(char *direccion) {

    t_config *una_config_kernel = config_create(direccion);
      	if(una_config_kernel == NULL){
	  error_log("configuracion_cpu.c@iniciar_config_cpu", "Ocurrio un error al intentar abrir el Archivo de Configuracion de KERNEL");
    }

    procesar_archivo_config_kernel(una_config_kernel);

    config_destroy(una_config_kernel);
}

void procesar_archivo_config_kernel(t_config *una_config_kernel) {
    info_log("config_kernel.c@procesar_archivo_config_kernel", "Se comienza a procesar Archivo de Configuracion de KERNEL ");
    int configElements = config_keys_amount(una_config_kernel);

    if (configElements == 0) {
       
        error_log(
                "config_kernel.c@procesar_archivo_config_kernel", "No hay claves en el archivo. - Archivo de configuración de KERNEL inválido.");
        exit(EXIT_FAILURE);
    } else {
        cargar_archivo_config_kernel(una_config_kernel);
    }
}


config_kernel_t* crear_estructura_kernel_config() {
    config_kernel_t* config_kernel = malloc(sizeof(config_kernel_t));
    config_kernel->ip_memoria = NULL;  
    config_kernel->puerto_memoria = NULL;  
    config_kernel->ip_cpu = NULL;   
    config_kernel->puerto_cpu_dispatch = NULL; 
    config_kernel->puerto_cpu_interrupt = NULL; 
    config_kernel->puerto_escucha = NULL; 
    config_kernel->ip_escucha=NULL;
    return config_kernel;
}

int obtener_int_arch_config(t_config *configuracion, char *clave) {
    int valor;
    if (config_has_property(configuracion, clave)) 
    {
        valor = config_get_int_value(configuracion, clave);

    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion Kernel incompleto. Falta clave Tipo Int");

    return valor;
}


double obtener_double_arch_config(t_config *configuracion, char *clave) {  

    double valor;

    if (config_has_property(configuracion, clave))
    {

        valor = config_get_double_value(configuracion, clave);

    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion Kernel incompleto. Falta clave Tipo Int");

    return valor;
}



void cargar_archivo_config_kernel(t_config *una_config_kernel) {
    info_log("config_kernel.c@cargar_archivo_config_kernel", "Se carga estructura de configuracion de Kernel");

    kernel_config = crear_estructura_kernel_config();

    kernel_config->ip_memoria =obtener_string_arch_config(una_config_kernel, "IP_MEMORIA");
    kernel_config->ip_cpu = obtener_string_arch_config(una_config_kernel, "IP_CPU");
    kernel_config->ip_escucha = obtener_string_arch_config(una_config_kernel, "IP_ESCUCHA");

    int puerto_mem = obtener_int_arch_config(una_config_kernel, "PUERTO_MEMORIA");
    char* puerto_memoria = string_itoa(puerto_mem);
    kernel_config->puerto_memoria = string_duplicate(puerto_memoria);
    free(puerto_memoria); 

    int puerto_cpu_dispatch =obtener_int_arch_config(una_config_kernel, "PUERTO_CPU_DISPATCH");
    char* puerto_dispat = string_itoa(puerto_cpu_dispatch);
    kernel_config->puerto_cpu_dispatch = string_duplicate(puerto_dispat);
    free(puerto_dispat);

    int puerto_cpu_interrupt = obtener_int_arch_config(una_config_kernel, "PUERTO_CPU_INTERRUPT");
    char* puerto_interrupt = string_itoa(puerto_cpu_interrupt);
    kernel_config->puerto_cpu_interrupt = string_duplicate(puerto_interrupt);
    free(puerto_interrupt); 

    int puerto_escucha = obtener_int_arch_config(una_config_kernel, "PUERTO_ESCUCHA");
    char* puerto_listen = string_itoa(puerto_escucha);
    kernel_config->puerto_escucha = string_duplicate(puerto_listen);
    free(puerto_listen); 

    char* algoritmo = obtener_string_arch_config(una_config_kernel, "ALGORITMO_PLANIFICACION");
    kernel_config->algoritmo_planificacion =parse_algoritmo_planificacion(algoritmo);
    free(algoritmo);
    
    kernel_config->estimacion_inicial = obtener_int_arch_config(una_config_kernel, "ESTIMACION_INICIAL");

    kernel_config->alfa = obtener_double_arch_config(una_config_kernel, "ALFA");

    kernel_config->grado_multiprogramacion =obtener_int_arch_config(una_config_kernel, "GRADO_MULTIPROGRAMACION");

    kernel_config->tiempo_maximo_bloqueado = obtener_int_arch_config(una_config_kernel, "TIEMPO_MAXIMO_BLOQUEADO");

}


char *obtener_string_arch_config(t_config *configuracion, char *clave) {
    char *valor = NULL;
    if (config_has_property(configuracion, clave)) {
        valor = string_duplicate(config_get_string_value(configuracion, clave)); 
        if (valor == NULL) {
            terminar_proceso_configuracion("Error: Archivo de configuracion sin "
                                           "valores definidos de Tipo String");
        }
    } else {
        terminar_proceso_configuracion("Error: Archivo de configuracion "
                                       "incompleto. Falta clave de tipo string ");
    }

    return valor;
}

void terminar_proceso_configuracion(char *MensajeError) {
    error_show(MensajeError); // Muestra un mensaje de error alertando del
    // problema
    printf("\n");
    exit(EXIT_FAILURE); // Finaliza el proceso
}

algoritmo_p_t parse_algoritmo_planificacion(char *algoritmo) {
    algoritmo_p_t algoritmo_planificacion;

    if (!strcmp(algoritmo, "FIFO")) {
  
        info_log("config_kernel.c@parse_algoritmo_planificacion", "Algoritmo de planificacion: FIFO");
        algoritmo_planificacion = FIFO;
    } else if (!strcmp(algoritmo, "SRT")) {
    
        info_log("config_kernel.c@parse_algoritmo_planificacion", "Algoritmo de planificacion: SRT");
        algoritmo_planificacion = SRT;
    } else {
        info_log("config_kernel.c@parse_algoritmo_planificacion"," Algoritmo de planificacion no disponible. Se setea por default FIFO");
        algoritmo_planificacion = FIFO;
    }

    return algoritmo_planificacion;
}

void destruir_estructura_kernel_config() {
    info_log("config_kernel.c@destruir_estructura_kernel_config" ,"Se destruye la estructura de configuracion de KERNEL");
    free(kernel_config->ip_memoria);
    free(kernel_config->puerto_memoria);
    free(kernel_config->ip_cpu);
    free(kernel_config->puerto_cpu_dispatch);
    free(kernel_config->puerto_cpu_interrupt);
    free(kernel_config->puerto_escucha);
    free(kernel_config->ip_escucha);
    free(kernel_config);
}

