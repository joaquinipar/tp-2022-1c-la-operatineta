#ifndef CONSOLA_CONFIG_CONSOLA_H
#define CONSOLA_CONFIG_CONSOLA_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/error.h>
#include <string.h>

typedef struct {
    char* ip_kernel;
    char* puerto_kernel;

} tipo_config_consola;

tipo_config_consola *consola_config;

void crear_estructura_consola_config(t_config *config);
int obtener_int_arch_config(t_config *configuracion, char *clave);
double obtener_double_arch_config(t_config *configuracion, char *clave);

void cargar_archivo_config_consola(t_config *config);
void procesar_archivo_config_consola(t_config *config);
void iniciar_config_consola(char *direccion);
char *obtener_string_arch_config(t_config *configuracion, char *clave);
void terminar_proceso_configuracion(char *MensajeError);
void print_config();
void destruir_estructura_consola_config();


#endif //CONSOLA_CONFIG_CONSOLA_H
