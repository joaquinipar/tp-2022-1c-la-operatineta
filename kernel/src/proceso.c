#include "../include/proceso.h"

pthread_mutex_t proceso_mutex;

void inicializar_monitor_procesos() {
	pthread_mutex_init(&proceso_mutex, NULL);
}

void proceso_destruir(pcb_t *proceso) {
  debug_log("proceso.c@proceso_destruir", "Destruyendo un pcb_t");

  list_destroy_and_destroy_elements(proceso->lista_instrucciones, free); // TODO: destruir elementos dentro de la lista si es necesario

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
  
  enviar_proceso_a_cpu(proceso);
}

pcb_t* proceso_obtener_tabla_paginas(pcb_t *proceso) {
  //enviar mensaje a memoria y acutalizar la tabla_pagins
  uint32_t valor_tabla_1er_nivel= enviar_mensaje_valor_tabla_1er_nivel(proceso->pid, proceso->tamanio); 
  proceso->tabla_paginas = valor_tabla_1er_nivel; 
  return proceso; 
}

void proceso_finalizar_rafaga(pcb_t *proceso) {
  gettimeofday(&(proceso->rafaga_actual).fin, NULL);
  proceso->duracion_ultima_rafaga = timedifference_msec((proceso->rafaga_actual).inicio, (proceso->rafaga_actual).fin);
  format_debug_log("proceso.c@proceso_finalizar_rafaga", "Proceso: %d tiempo total ultima rafaga", proceso->duracion_ultima_rafaga);
  proceso->estimacion = proxima_rafaga_estimada(proceso);
  format_debug_log("proceso.c@proceso_finalizar_rafaga", "Proceso: %d proxima rafaga estimada", proceso->estimacion);
}

void proceso_actualizar_rafaga_por_desalojo(pcb_t *proceso){
  
  gettimeofday(&(proceso->rafaga_actual).fin, NULL);
  int duracion_ultima_rafaga = timedifference_msec((proceso->rafaga_actual).inicio, (proceso->rafaga_actual).fin);
  proceso->estimacion = proceso->estimacion -  duracion_ultima_rafaga;
  format_debug_log("proceso.c@proceso_actualizar_rafaga_por_desalojo", "Proceso: %d tiempo restante de rafaga", proceso->estimacion);

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