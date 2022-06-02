#ifndef KERNEL_INCLUDE_PLANIFICADOR_MEDIANO_PLAZO_H_
#define KERNEL_INCLUDE_PLANIFICADOR_MEDIANO_PLAZO_H_

#include <pthread.h>
#include <utils/structs.h>

pthread_t planificador_mediano_plazo;

void iniciar_planificador_mediano_plazo(); 
void finalizar_planificador_mediano_plazo();
void suspender_procesos(); 

#endif /* KERNEL_INCLUDE_PLANIFICADOR_MEDIANO_PLAZO_H_ */
