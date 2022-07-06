#include "../include/planificador_mediano_plazo.h"

pthread_t planificador_mediano_plazo;

void suspender_proceso_despues_de_tiempo_maximo(pcb_t *proceso);
void despertar_procesos();

void iniciar_planificador_mediano_plazo() {
  debug_log("planificador_mediano_plazo.c@iniciar_planificador_mediano_plazo", "Iniciando planificador mediano plazo");
  pthread_create(&planificador_mediano_plazo, NULL, (void *)despertar_procesos, NULL);
  pthread_detach(planificador_mediano_plazo);
}

void finalizar_planificador_mediano_plazo() {
  pthread_cancel(planificador_mediano_plazo);
}

void despertar_procesos() {

  while (1) {

    // Hay un proceso suspendido
    sem_wait(&sem_proceso_suspendido);
    // Se libera un grado de multiprogramacion
    //sem_wait(&sem_grado_multiprogramacion_disponible);
    info_log("planificador_mediano_plazo.c@despertar_procesos", "Plani de mediano plazo despertado");
    //mover_proceso_suspendido_a_listo();
    mover_proceso_a_listo();

  }
}

/**
 * 
 * Avisa cuando un proceso termino de usar un dispositivo de IO
 * 
 * */
void io_dispositivo_finalizada(pcb_t *proceso) {

  format_info_log("planificador_mediano_plazo.c@io_dispositivo_finalizada", "La io del proceso con pid: %d ha finalizado, se movera el proceso a (suspendido) listo", proceso->pid);
  // TODO me parece que en realidad hay que checkear tambien si el proceso esta bloqueado-suspendido o solo bloqueado
  if (proceso->estado == ESTADO_PROCESO_BLOCKED_SUSPENDED) {
    encolar_proceso_en_suspendidos_listos(proceso);
  } else {
	  encolar_proceso_en_listos(proceso);
  }

}

// A efectos del planificador, solo suspendemos un proceso si pasa bloqueado mas del tiempo maximo configurado
// debe enviar el mensaje a memoria para suspender el proceso
void lanzar_thread_suspension_proceso(pcb_t *proceso) {
  format_debug_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Lanzando thread de suspension de proceso %d", proceso->pid);
  pthread_t thread_suspension;

  pthread_create(&thread_suspension, NULL, (void *)suspender_proceso_despues_de_tiempo_maximo, proceso);
  pthread_detach(thread_suspension);
}

void suspender_proceso_despues_de_tiempo_maximo(pcb_t *proceso) {
  usleep(kernel_config->tiempo_maximo_bloqueado * 1000);
  format_debug_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Chequeando si tengo que dormir proceso %d (despues del tiempo maximo de espera)", proceso->pid);
  if (proceso->estado == ESTADO_PROCESO_BLOCKED) {
    format_info_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Paso el tiempo maximo, el proceso sigue bloqueado lo tengo que dormir, pid: %d", proceso->pid);

    // Este mensaje es bloqueante asi que espera a que memoria responda y luego cambia el estado y hace el signal
    enviar_mensaje_suspender_proceso(proceso);

    proceso->estado = ESTADO_PROCESO_BLOCKED_SUSPENDED;
    sem_post(&sem_grado_multiprogramacion_disponible);

  } else {
    format_debug_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Paso el tiempo maximo y al proceso: %d no sigue bloqueado, no hace falta dormir", proceso->pid);
  }
}