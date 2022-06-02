#include "../include/planificador_mediano_plazo.h"


void iniciar_planificador_mediano_plazo() {

  pthread_create(&planificador_mediano_plazo, NULL, (void *)suspender_procesos, NULL);
  pthread_detach(planificador_mediano_plazo);
}

void finalizar_planificador_mediano_plazo() {

  pthread_cancel(planificador_mediano_plazo);
}

void suspender_procesos() {

  while (1) {

      //logica para suspender proceso
      //No olvidar mandar mensaje a memoria

  }
}

/**
 * 
 * Avisa cuando un proceso termino de usar un dispositivo de IO
 * 
 * */
void io_dispositivo_finalizada(pcb_t *proceso) {

  // TODO me parece que en realidad hay que checkear tambien si el proceso esta bloqueado-suspendido o solo bloqueado
  // Si no hay lugar en el grado de multiprogramacion tiene que ir a suspendido/listos
  if (grado_multiprogramacion_completo()) {
    // TODO: avisar a memoria que hay que suspender el proceso
    encolar_proceso_en_suspendidos_listos(proceso);
  }
  else
  {
	  encolar_proceso_en_listos(proceso);
  }

}