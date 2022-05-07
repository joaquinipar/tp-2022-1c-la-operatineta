#include "config_consola.h"


void iniciar_config_consola(char *direccion) {

    t_config *una_config_consola = config_create(direccion);

    procesar_archivo_config_consola(una_config_consola);

    config_destroy(una_config_consola);
}


void procesar_archivo_config_consola(t_config *una_config_consola) {
   info_log("config_consola.c@cargar_archivo_config_consola", "Se comienza a procesar Archivo de Configuracion de CONSOLA");
   int configElements = config_keys_amount(una_config_consola);

    if (configElements == 0) {
        error_log("config_consola.c@procesar_archivo_config_consola", "No hay claves en el archivo - Archivo de configuración inválido");
        exit(EXIT_FAILURE);
    } else {
        cargar_archivo_config_consola(una_config_consola);
    }
}

void cargar_archivo_config_consola(t_config *una_config_consola) {
   
    consola_config = crear_estructura_config();

    consola_config->ip_kernel = obtener_string_arch_config(una_config_consola, "IP_KERNEL");

    int puerto_kernel = obtener_int_arch_config(una_config_consola, "PUERTO_KERNEL");
    char* str_puerto_kernel = string_itoa(puerto_kernel);
    consola_config->puerto_kernel = string_duplicate(str_puerto_kernel);

    free(str_puerto_kernel);
}

tipo_config_consola *crear_estructura_config() {
    tipo_config_consola *config = malloc(sizeof(tipo_config_consola));
    config->ip_kernel = NULL;
    return config;
}

int obtener_int_arch_config(t_config *configuracion, char *clave) {
    int valor;
    if (config_has_property(configuracion, clave)) 
    {
        valor = config_get_int_value(configuracion, clave); 

    } else
        terminar_proceso_configuracion("Error: Archivo de configuracion Kernel "
                                       "incompleto. Falta clave Tipo Int");

    return valor;
}


char *obtener_string_arch_config(t_config *configuracion, char *clave) {
    char *valor = NULL;
    if (config_has_property(configuracion, clave)) 
    {
        valor = string_duplicate(config_get_string_value(
                configuracion, clave)); 
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

    info_log("config_consola.c@destruir_estructura_consola_config", "Se destruye la estructura de configuracion de CONSOLA");
    free(consola_config->ip_kernel);
    free(consola_config);
}

void print_config() {
    debug_log("config_consola.c@print_config",consola_config->ip_kernel);
    debug_log("config_consola.c@print_config", consola_config->puerto_kernel);
}