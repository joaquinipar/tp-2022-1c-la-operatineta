#include "../include/planificador_corto_plazo.h"

pthread_t hilo_procesador;

void iniciar_planificador_corto_plazo() {

    pthread_create(&hilo_procesador, NULL, (void *)planificar_procesos,
                   NULL);
    pthread_detach(hilo_procesador);


}

void planificar_procesos() {

  while (1) {

    sem_wait(&sem_proceso_listo); // (contador) proceso nuevo a ready
    sem_wait(&sem_bin_procesar_listo); // (binario) indica si se puede ejecutar un proceso
    info_log("planificador_corto_plazo@planificar_procesos", "Moviendo proceso listo a ejecucion");
    
    pcb_t *proceso = mover_proceso_listo_a_ejecucion();

  }
}

void replanificar_procesos(){
  if(kernel_config -> algoritmo_planificacion == FIFO){

    info_log("planificador_corto_plazo.c@replanificar_procesos", "replanificando procesos");
    sem_post(&sem_bin_procesar_listo);
  }
}


void finalizar_planificador_corto_plazo() {

  pthread_cancel(hilo_procesador);

}