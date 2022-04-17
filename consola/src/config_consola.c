#include "config_consola.h"


tipo_config_consola *crear_estructura_config() {
    tipo_config_consola *config = malloc(sizeof(tipo_config_consola));

    config->ip_kernel = NULL;
    config->puerto_kernel = NULL;
    return config;
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

double obtener_double_arch_config(t_config *configuracion, char *clave) {

    double valor;

    if (config_has_property(configuracion, clave))
    {

        valor = config_get_double_value(configuracion, clave);

    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion Consola "
                                       "incompleto. Falta clave Tipo Int");

    return valor;
}



void cargar_archivo_config_consola(t_config *config) {
    // TODO Log
    printf("config_consola.c@cargar_archivo_config_consola Se carga estructura de configuracion de Consola");

    consola_config = crear_estructura_config();

    consola_config->ip_kernel =
            obtener_string_arch_config(config, "IP_KERNEL");

    consola_config->puerto_kernel =
            obtener_string_arch_config(config, "PUERTO_KERNEL");

}

void crear_estructura_consola_config(t_config *config) {
    // TODO Log
    printf("config_consola.c@procesar_archivo_config_consola | Se comienza a procesar Archivo de Configuracion");
    int configElements = config_keys_amount(config);

    if (configElements == 0) {
        // TODO Log error
        printf(
                "config_consola.c@procesar_archivo_config_consola | No hay claves en el archivo. - Archivo de configuración inválido.");
        exit(EXIT_FAILURE);
    } else {
        cargar_archivo_config_consola(config);
    }
}

void procesar_archivo_config_consola(t_config *config) {
    // TODO Log
    printf("config_consola.c@procesar_archivo_config_consola | Se comienza a procesar Archivo de Configuracion");
    int configElements = config_keys_amount(config);

    if (configElements == 0) {
        // TODO Log error
        printf(
                "config_consola.c@procesar_archivo_config_consola | No hay claves en el archivo. - Archivo de configuración inválido.");
        exit(EXIT_FAILURE);
    } else {
        cargar_archivo_config_consola(config);
    }
}



void iniciar_config_consola(char *direccion) {

    t_config *config_consola = config_create(direccion);

    procesar_archivo_config_consola(config_consola);

    config_destroy(config_consola);
}

char *obtener_string_arch_config(t_config *configuracion, char *clave) {
    char *valor = NULL;
    if (config_has_property(configuracion, clave)) // Comprueba que el archivo de
        // configuracion contenga la
        // clave solicitada.
    {
        valor = string_duplicate(config_get_string_value(
                configuracion, clave)); // se le asigna a la variable valor el valor
        // correspondiente a la clave solicitada
        if (valor == NULL)
            terminar_proceso_configuracion("Error: Archivo de configuracion sin "
                                           "valores definidos de Tipo String");
    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion "
                                       "incompleto. Falta clave de tipo string ");

    return valor;
}

void terminar_proceso_configuracion(char *MensajeError) {
    error_show(MensajeError); // Muestra un mensaje de error alertando del
    // problema
    printf("\n");
    exit(EXIT_FAILURE); // Finaliza el proceso
}


void destruir_estructura_consola_config() {
    // TODO Log
    printf("config_consola.c@destruir_estructura_consola_config Se destruye la estructura de configuracion");


    free(consola_config->ip_kernel);
    free(consola_config->puerto_kernel);



    free(consola_config);
}

void print_config() {
    printf("\n");
    printf("%s\n",consola_config->ip_kernel);
    printf("%s\n",consola_config->puerto_kernel);
}