#include "../include/planificador_mediano_plazo.h"

void suspender_proceso_despues_de_tiempo_maximo(pcb_t *proceso);
void despertar_procesos();

void iniciar_planificador_mediano_plazo() {

  pthread_create(&planificador_mediano_plazo, NULL, (void *)despertar_procesos, NULL);
  pthread_detach(planificador_mediano_plazo);
}

void finalizar_planificador_mediano_plazo() {

  pthread_cancel(planificador_mediano_plazo);
}

void despertar_procesos() {

  while (1) {

    // TODO se libero un grado de multiprogramacion, ver si entra un suspendido listo a listos
    // esto se debe postear cuando un proceso hace exit o se suspende uno bloqueado (agregar el post en esos archivos)
    //wait(&sem_grado_multiprogramacion_liberado);
    

  }
}

/**
 * 
 * Avisa cuando un proceso termino de usar un dispositivo de IO
 * 
 * */
void io_dispositivo_finalizada(pcb_t *proceso) {

  // TODO me parece que en realidad hay que checkear tambien si el proceso esta bloqueado-suspendido o solo bloqueado
  if (proceso->estado == ESTADO_PROCESO_BLOCKED_SUSPENDED) {
    encolar_proceso_en_suspendidos_listos(proceso);

    //sem_post(&sem_proceso_nuevo); // esto llama a el plani de largo, tenemos la logica de mover proceso suspendido-listo o nuevo a ejecucion
  }
  else
  {
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
  sleep(kernel_config->tiempo_maximo_bloqueado);
  format_debug_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Chequeando si tengo que dormir proceso %d", proceso->pid);
  if (proceso->estado == ESTADO_PROCESO_BLOCKED) {
    format_debug_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Paso el tiempo maximo y tengo que dormir al proceso: %d", proceso->pid);
    proceso->estado = ESTADO_PROCESO_BLOCKED_SUSPENDED;
    enviar_mensaje_suspender_proceso(proceso);
  } else {
    format_debug_log("planificador_mediano_plazo.c@lanzar_thread_suspension_proceso", "Paso el tiempo maximo y al proceso: %d no estaba bloqueado, no hace falta dormir", proceso->pid);
  }
}