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