#ifndef MONITOR_COLAS_PROCESOS_H
#define MONITOR_COLAS_PROCESOS_H

#include "config_kernel.h"
#include <utils/logger.h>
#include <commons/collections/list.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include "structs.h"
#include "proceso.h"
#include "planificador_corto_plazo.h"

void agregar_proceso_a_lista_de_procesos(pcb_t *proceso);
pcb_t *buscar_proceso(uint32_t pid);

// Operaciones de inicializacion/destruccion
void iniciar_monitor_colas_procesos();
void destruir_monitor_colas_procesos();

// Operaciones sobre cola de nuevos procesos
bool encolar_proceso_en_nuevos(pcb_t *proceso);
pcb_t *desencolar_proceso_nuevo();
pcb_t *buscar_proceso_en_nuevos(int);
bool lista_de_nuevos_vacia();

// Operaciones sobre cola de listos
void encolar_proceso_en_listos(pcb_t *proceso);
pcb_t *desencolar_proceso_listo();
void ordenar_cola_listos();
pcb_t* buscar_proceso_en_listos(int);
bool lista_de_listos_vacia();

// Operaciones sobre cola de ejecucion
void encolar_proceso_en_ejecucion(pcb_t *proceso);
pcb_t *desencolar_proceso_en_ejecucion();
pcb_t* buscar_proceso_en_ejecucion(int);

// Operaciones sobre cola de bloqueados
void encolar_proceso_en_bloqueados(pcb_t* proceso);
pcb_t* desencolar_proceso_bloqueado();
pcb_t* obtener_ultimo_proceso_bloqueado();
pcb_t* buscar_y_sacar_proceso_en_bloqueados(int);
bool lista_de_bloqueados_vacia();

// Operaciones sobre cola de bloqueados suspendidos
void encolar_proceso_en_bloqueados_suspendidos(pcb_t *proceso);
pcb_t *desencolar_proceso_bloqueado_suspendido();
pcb_t* buscar_y_sacar_proceso_en_bloqueados_suspendidos(int);

// Operaciones sobre cola de suspendidos listos
void encolar_proceso_en_suspendidos_listos(pcb_t* proceso);
pcb_t* desencolar_proceso_suspendido_listo();
pcb_t* buscar_proceso_en_suspendidos_listos();
bool lista_de_suspendidos_listos_vacia();

// Operaciones sobre cola de terminados
void encolar_proceso_en_terminados(pcb_t *proceso);

// Otras funciones
void mover_proceso_a_listo();
pcb_t *mover_proceso_listo_a_ejecucion();
void mover_ultimo_proceso_bloqueado_a_suspendido();
void mover_proceso_ejecutando_a_terminados();
int cantidad_procesos_en_multiprogramacion();
bool mayor_prioridad(pcb_t *, pcb_t *);
int proxima_rafaga_estimada(pcb_t *);
int response_ratio(pcb_t *);
void actualizar_espera_listos();
pcb_t *buscar_y_sacar_proceso_de_cualquier_cola(uint32_t);
bool mover_proceso_en_ejecucion_a_bloqueado(int proceso);
bool grado_multiprogramacion_completo();
void mover_proceso_de_bloqueados_a_terminados(uint32_t pid);
void mover_proceso_nuevo_a_suspendido_listo();

// Funciones a eliminar quizás
int cantidad_procesos_nuevos();
int cantidad_procesos_listos();
int cantidad_procesos_ejecutando();
int cantidad_procesos_bloqueados();
int cantidad_procesos_bloqueados_suspendidos();
int cantidad_procesos_suspendidos_listos();
int cantidad_procesos_terminados();

sem_t sem_proceso_nuevo;
sem_t sem_proceso_listo;
sem_t sem_grado_multiprogramacion_completo;
sem_t sem_proceso_suspendido;
sem_t sem_bin_procesar_listo;

// Evalua si hay posibilidad de interbloqueo
bool esta_en_estado_seguro();

int get_cantidad_procesos();
void procesos_lock();
void procesos_unlock();

t_list_iterator *procesos_get_iterator();

void mover_proceso_en_ejecucion_a_terminados(uint32_t pid);

t_list *get_procesos();
t_list *get_procesos_bloqueados();

#endif
