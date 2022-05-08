#include "../include/planificador_corto_plazo.h"

pthread_t hilo_procesador;

void iniciar_planificador_corto_plazo() {

    pthread_create(&hilo_procesador, NULL, (void *)planificar_procesos,
                   NULL);
    pthread_detach(hilo_procesador);


}

void planificar_procesos() {

  while (1) {

    sem_wait(&sem_proceso_listo);
    sem_wait(&sem_bin_procesar_listo);

    pcb_t *proceso = mover_proceso_listo_a_ejecucion();

    //esperar_mensaje_cpu();

  }
}

void finalizar_planificador_corto_plazo() {

  pthread_cancel(hilo_procesador);

}