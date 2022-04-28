#include "../include/proceso.h"

pcb_t* crear_proceso(int pid, int socket) {
  debug_log("proceso.c@crear_proceso", "Inicializando un pcb_t");
  pcb_t* nuevo_proceso = malloc(sizeof(pcb_t));
  nuevo_proceso->pid = pid;
  nuevo_proceso->socket = socket;
  nuevo_proceso->estado = ESTADO_PROCESO_NEW;
  //nuevo_proceso->estimacion_anterior = kernel_config->estimacion_inicial;
  //nuevo_proceso->ultima_rafaga = 0;
  //nuevo_proceso->tiempo_espera = 0;
  //nuevo_proceso->rafaga = malloc(sizeof(rango_tiempo_t));
  //nuevo_proceso->espera = malloc(sizeof(rango_tiempo_t)); 
  //printf("Estimación incial: %d", nuevo_proceso->estimacion_anterior);
  debug_log("proceso.c@crear_proceso", "Devolviendo el proceso_t inicializado");
  return nuevo_proceso;
}

void proceso_destruir(pcb_t *proceso) {
  debug_log("proceso.c@proceso_destruir", "Destruyendo un pcb_t");


  free(proceso);

  debug_log("proceso.c@proceso_destruir", "Pcb_t destruido");
}
