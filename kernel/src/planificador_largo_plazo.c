#include "../include/planificador_largo_plazo.h"

static pthread_t planificador_largo_plazo;

void iniciar_planificador_largo_plazo() {

  pthread_create(&planificador_largo_plazo, NULL,
                 (void *)controlar_grado_de_multiprogramacion, NULL);
  pthread_detach(planificador_largo_plazo);

}

void controlar_grado_de_multiprogramacion() {

  info_log("planificador_largo_plazo.c@controlar_grado_de_multiprogramacion", "Iniciar planificador largo plazo");

  while (1) {

    sem_wait(&sem_proceso_nuevo);

    if (!grado_multiprogramacion_completo()) {

      info_log("planificador_largo_plazo.c@controlar_grado_de_multiprogramacion", "Grado de multiprogramacion no esta completo, moviendo un proceso a ready");
      mover_proceso_a_listo();

    } else {

      info_log("planificador_largo_plazo.c@controlar_grado_de_multiprogramacion", "Grado de multiprogramacion COMPLETO, moviendo un proceso a suspended-ready");
      mover_proceso_nuevo_a_suspendido_listo();

    }

  }
}

void finalizar_planificador_largo_plazo() {
  pthread_cancel(planificador_largo_plazo);
}