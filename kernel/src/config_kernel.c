#include "config_kernel.h"

tipo_config_kernel *crear_estructura_kernel_config() {
    tipo_config_kernel *config_kernel = malloc(sizeof(tipo_config_kernel));
    config_kernel->ip_memoria = NULL;  
    config_kernel->puerto_memoria = NULL;   //es numerico
    config_kernel->ip_cpu = NULL;   
    config_kernel->puerto_cpu_dispatch = NULL; //es numerico
    config_kernel->puerto_cpu_interrupt = NULL; //es numerico
    config_kernel->puerto_escucha = NULL; //es numerico
    /*
    config_kernel->algoritmo_planificacion = -1;
    config_kernel->estimacion_inicial = -1;
    config_kernel->alfa = -1;
    config_kernel->grado_multiprogramacion = NULL;
    config_kernel->tiempo_maximo_bloqueado = NULL;
    */
    return config_kernel;
}

int obtener_int_arch_config(t_config *configuracion, char *clave) {
    int valor;
    if (config_has_property(configuracion, clave)) // Comprueba que el archivo de
        // configuracion contenga la
        // clave solicitada.
    {
        valor = config_get_int_value(configuracion, clave); // se le asigna a la
        // variable valor el
        // valor correspondiente
        // a la clave solicitada

    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion Kernel "
                                       "incompleto. Falta clave Tipo Int");

    return valor;
}


//ESta funcion para que esta ¡??? 

double obtener_double_arch_config(t_config *configuracion, char *clave) {  

    double valor;

    if (config_has_property(configuracion, clave))
    {

        valor = config_get_double_value(configuracion, clave);

    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion Kernel "
                                       "incompleto. Falta clave Tipo Int");

    return valor;
}



void cargar_archivo_config_kernel(t_config *una_config_kernel) {
    // TODO Log
    printf("config_kernel.c@cargar_archivo_config_kernel Se carga estructura de configuracion de Kernel");

    kernel_config = crear_estructura_kernel_config();

    kernel_config->ip_memoria =
            obtener_string_arch_config(una_config_kernel, "IP_MEMORIA");

    kernel_config->puerto_memoria =
            obtener_string_arch_config(una_config_kernel, "PUERTO_MEMORIA");

    kernel_config->ip_cpu =
            obtener_string_arch_config(una_config_kernel, "IP_CPU");

    kernel_config->puerto_cpu_dispatch =
            obtener_string_arch_config(una_config_kernel, "PUERTO_CPU_DISPATCH");

    kernel_config->puerto_cpu_interrupt =
            obtener_string_arch_config(una_config_kernel, "PUERTO_CPU_INTERRUPT");

    kernel_config->puerto_escucha =
            obtener_string_arch_config(una_config_kernel, "PUERTO_ESCUCHA");

    kernel_config->algoritmo_planificacion =
        parse_algoritmo_planificacion(obtener_string_arch_config(una_config_kernel, "ALGORITMO_PLANIFICACION"));

    kernel_config->estimacion_inicial =
            obtener_int_arch_config(una_config_kernel, "ESTIMACION_INICIAL");

    kernel_config->alfa =
            obtener_double_arch_config(una_config_kernel, "ALFA");

    kernel_config->grado_multiprogramacion =
            obtener_int_arch_config(una_config_kernel, "GRADO_MULTIPROGRAMACION");

    kernel_config->tiempo_maximo_bloqueado =
            obtener_int_arch_config(una_config_kernel, "TIEMPO_MAXIMO_BLOQUEADO");

    

}

void procesar_archivo_config_kernel(t_config *una_config_kernel) {
    // TODO Log
    printf("config_kernel.c@procesar_archivo_config_kernel | Se comienza a procesar Archivo de Configuracion");
    int configElements = config_keys_amount(una_config_kernel);

    if (configElements == 0) {
        // TODO Log error
        printf(
                "config_kernel.c@procesar_archivo_config_kernel | No hay claves en el archivo. - Archivo de configuración inválido.");
        exit(EXIT_FAILURE);
    } else {
        cargar_archivo_config_kernel(una_config_kernel);
    }
}


void iniciar_config_kernel(char *direccion) {

    t_config *una_config_kernel = config_create(direccion);

    procesar_archivo_config_kernel(una_config_kernel);

    config_destroy(una_config_kernel);
}

char *obtener_string_arch_config(t_config *configuracion, char *clave) {
    char *valor = NULL;
    if (config_has_property(configuracion, clave)) {
        valor = string_duplicate(config_get_string_value(
                configuracion, clave)); 
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

algoritmo_p parse_algoritmo_planificacion(char *algoritmo) {
    algoritmo_p algoritmo_planificacion;

    if (!strcmp(algoritmo, "FIFO")) {
        // TODO Log
        printf("config_kernel.c@parse_algoritmo_planificacion Algoritmo de planificacion: FIFO");
        algoritmo_planificacion = FIFO;
    } else if (!strcmp(algoritmo, "SRT")) {
        // TODO Log
        printf("config_kernel.c@parse_algoritmo_planificacion Algoritmo de planificacion: SRT");
        algoritmo_planificacion = SRT;
    } else {
        // TODO Log
        printf("config_kernel.c@parse_algoritmo_planificacion Algoritmo de planificacion no disponible. Se setea por default FIFO");
        algoritmo_planificacion = FIFO;
    }

    return algoritmo_planificacion;
}

void destruir_estructura_kernel_config() {
    // TODO Log
    printf("config_kernel.c@destruir_estructura_kernel_config Se destruye la estructura de configuracion");


    free(kernel_config->ip_memoria);
    free(kernel_config->puerto_memoria);
    free(kernel_config->ip_cpu);
    free(kernel_config->puerto_cpu_dispatch);
    free(kernel_config->puerto_cpu_interrupt);
    free(kernel_config->puerto_escucha);


    free(kernel_config);
}

void print_config() {
    printf("\n");
    printf("%s\n",kernel_config->ip_memoria);
    printf("%s\n",kernel_config->puerto_memoria);
    printf("%s\n",kernel_config->ip_cpu);
    printf("%s\n",kernel_config->puerto_cpu_dispatch);
    printf("%s\n",kernel_config->puerto_cpu_interrupt);
    printf("%s\n",kernel_config->puerto_escucha);
    printf("%i\n",kernel_config->algoritmo_planificacion);
    printf("%i\n",kernel_config->estimacion_inicial );
    printf("%f\n",kernel_config->alfa);
    printf("%i\n",kernel_config->grado_multiprogramacion);
    printf("%i\n",kernel_config->tiempo_maximo_bloqueado);
}