#include "../include/planificador_largo_plazo.h"

static pthread_t planificador_largo_plazo;
void admitir_proceso_nuevo();

void iniciar_planificador_largo_plazo() {

  pthread_create(&planificador_largo_plazo, NULL,
                 (void *)admitir_proceso_nuevo, NULL);
  pthread_detach(planificador_largo_plazo);

}

void admitir_proceso_nuevo() {

  info_log("planificador_largo_plazo.c@admitir_proceso_nuevo", "Iniciar planificador largo plazo");

  while (1) {

    // Este semaforo se llama cuando viene un proceso nuevo
    sem_wait(&sem_proceso_nuevo);
    sem_wait(&sem_grado_multiprogramacion_disponible);

    info_log("planificador_largo_plazo.c@admitir_proceso_nuevo", "Moviendo un proceso de nuevo a ready");
    //pcb_t *proceso = desencolar_proceso_nuevo();
    mover_proceso_a_listo();

  }
}

void finalizar_planificador_largo_plazo() {
  pthread_cancel(planificador_largo_plazo);
}