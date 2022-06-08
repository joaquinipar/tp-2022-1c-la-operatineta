#ifndef KERNEL_INCLUDE_PLANIFICADOR_MEDIANO_PLAZO_H_
#define KERNEL_INCLUDE_PLANIFICADOR_MEDIANO_PLAZO_H_

#include <pthread.h>
#include <utils/structs.h>
#include "config_kernel.h"
#include "conexion_client.h"
#include "monitor_colas_procesos.h"

sem_t sem_grado_multiprogramacion_liberado;


void iniciar_planificador_mediano_plazo(); 
void finalizar_planificador_mediano_plazo();
void lanzar_thread_suspension_proceso(pcb_t *proceso);
void io_dispositivo_finalizada(pcb_t *proceso);

#endif /* KERNEL_INCLUDE_PLANIFICADOR_MEDIANO_PLAZO_H_ */
