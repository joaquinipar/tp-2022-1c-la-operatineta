#include "../include/proceso.h"

static void proceso_reanudar_ejecucion_remota();

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
  // TODO: implementar
}

void proceso_ejecutar(pcb_t *proceso) {

  pthread_mutex_lock(&proceso_mutex);
  proceso->estado = ESTADO_PROCESO_EXEC;
  proceso_iniciar_rafaga(proceso);

  pthread_mutex_unlock(&proceso_mutex);
}

void proceso_finalizar_rafaga(pcb_t *proceso) {
  //TODO: implementar
  /* gettimeofday(&proceso->rafaga->fin, NULL);
  proceso->ultima_rafaga =
      timedifference_msec(proceso->rafaga->inicio, proceso->rafaga->fin);
 */
  //printf("\n\nTiempo de ráfaga: %d\n\n", proceso->ultima_rafaga);

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