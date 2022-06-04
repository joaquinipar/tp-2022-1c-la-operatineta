#include "../include/proceso.h"

pthread_mutex_t proceso_mutex;

void inicializar_monitor_procesos() {
	pthread_mutex_init(&proceso_mutex, NULL);
}

void proceso_destruir(pcb_t *proceso) {
  debug_log("proceso.c@proceso_destruir", "Destruyendo un pcb_t");

  list_destroy(proceso->lista_instrucciones); // TODO: destruir elementos dentro de la lista si es necesario

  free(proceso);

  debug_log("proceso.c@proceso_destruir", "Proceso_t destruido");
}

void proceso_iniciar_rafaga(pcb_t *proceso) {
  gettimeofday(&(proceso->rafaga_actual).inicio, NULL);
}

void proceso_ejecutar(pcb_t *proceso) {

  pthread_mutex_lock(&proceso_mutex);
  proceso->estado = ESTADO_PROCESO_EXEC;
  proceso_iniciar_rafaga(proceso);
  pthread_mutex_unlock(&proceso_mutex);
  // TODO: agregar funcion proceso_obtener_tabla_paginas();
  enviar_proceso_a_cpu(proceso);
}

void proceso_obtener_tabla_paginas(pcb_t *proceso) {
  //enviar mensaje a memoria y acutalizar la tabla_pagins
  uint32_t tabla_paginas = 12; // deberia ser obtener_tabla_paginas();
  proceso->tabla_paginas = tabla_paginas;
}

// TODO: IMPLEMENTAR 
void proceso_bloquear(pcb_t *proceso, int tiempo_bloqueado) {
  // Si TIEMPO_MAXIMO_BLOQUEADO >= tiempo_bloqueado
  //      usleep(tiempo_bloqueado)
  //      mover a ready
  // else
  //    usleep(TIEMPO_MAXIMO_BLOQUEADO);
  //    mover_a_suspendido_bloqueado(pcb_t *proceso)
  //    usleep(tiempo_bloqueado - TIEMPO_MAXIMO_BLOQUEADO);
}

void proceso_finalizar_rafaga(pcb_t *proceso) {
  //TODO: implementar
  gettimeofday(&(proceso->rafaga_actual).fin, NULL);
  proceso->duracion_ultima_rafaga = timedifference_msec((proceso->rafaga_actual).inicio, (proceso->rafaga_actual).fin);
  format_debug_log("proceso.c@proceso_finalizar_rafaga", "Proceso: %d tiempo total ultima rafaga", proceso->duracion_ultima_rafaga);
  proceso->estimacion = proxima_rafaga_estimada(proceso);
  format_debug_log("proceso.c@proceso_finalizar_rafaga", "Proceso: %d proxima rafaga estimada", proceso->estimacion);
}

/* void proceso_iniciar_espera(pcb_t *proceso) {
  gettimeofday(&proceso->espera->inicio, NULL);
}

void proceso_finalizar_espera(pcb_t *proceso) {

  gettimeofday(&proceso->espera->fin, NULL);
  proceso->tiempo_espera =
      timedifference_msec(proceso->espera->inicio, proceso->espera->fin);
} */

float timedifference_msec(struct timeval t0, struct timeval t1) {
  return (t1.tv_sec - t0.tv_sec) * 1000.0f +
         (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
