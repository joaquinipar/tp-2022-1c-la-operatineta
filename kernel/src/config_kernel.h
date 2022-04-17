#ifndef KERNEL_CONFIG_KERNEL_H
#define KERNEL_CONFIG_KERNEL_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/error.h>
#include <string.h>

typedef enum {FIFO, SRT} algoritmo_p;

typedef struct tipo_config_kernel {
    char* ip_memoria;
    char* puerto_memoria;
    char* ip_cpu;
    char* puerto_cpu_dispatch;
    char* puerto_cpu_interrupt;
    char* puerto_escucha;
    algoritmo_p algoritmo_planificacion;
    int estimacion_inicial;
    double alfa;
    int grado_multiprogramacion;
    int tiempo_maximo_bloqueado;

} tipo_config_kernel;

tipo_config_kernel *kernel_config;

tipo_config_kernel *crear_estructura_kernel_config();
int obtener_int_arch_config(t_config *configuracion, char *clave);
double obtener_double_arch_config(t_config *configuracion, char *clave);

void cargar_archivo_config_kernel(t_config *una_config_kernel);
void procesar_archivo_config_kernel(t_config *una_config_kernel);
void iniciar_config_kernel(char *direccion);
char *obtener_string_arch_config(t_config *configuracion, char *clave);
void terminar_proceso_configuracion(char *MensajeError);
algoritmo_p parse_algoritmo_planificacion(char *algoritmo);
void print_config();

#endif //KERNEL_CONFIG_KERNEL_H
