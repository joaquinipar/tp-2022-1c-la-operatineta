#include "../include/kernel_api.h"

pid_t pid_count = 0;
pthread_mutex_t pid_mutex;
//TODO : creo que esto no es thread safe, no sé si es necesario que lo sea
//pthread_mutex_t mutex_pid;

void inicializar_kernel_api () {
  pthread_mutex_init(&pid_mutex, NULL);
}

pid_t obtener_siguiente_pid () {
  pthread_mutex_lock(&pid_mutex);
  pid_count++;
  pthread_mutex_unlock(&pid_mutex);

  return pid_count;
}

bool ejecutar_proceso_nuevo(pcb_t *proceso) {

  format_info_log("kernel/kernel_api@admitir_proceso_nuevo", "Admitiendo proceso nuevo con id: %d al sistema", proceso->pid);

  agregar_proceso_a_lista_de_procesos(proceso);

  return encolar_proceso_en_nuevos(proceso);

}

bool finalizar_proceso(pcb_t *proceso_actualizado) {

  pcb_t *proceso = buscar_proceso(proceso_actualizado->pid);
  proceso_finalizar_rafaga(proceso);
  proceso->estado = EXIT;

  pcb_t *proceso_desencolado = desencolar_proceso_en_ejecucion();

  if (proceso_desencolado->pid != proceso->pid) {
    warning_log("kernel_api.c@finalizar_proceso", "CUIDADO, MANQUEADA EN PROCESO TERMINADO (el id del proceso en ejecucion no coincide con el proceso que llego por dispatch)");
  }

  encolar_proceso_en_terminados(proceso);

  // TODO: enviar mensaje a memoria para eliminar proceso de memoria

  // TODO: revisar posible condicion de carrera, si no se ejecuta el plani de largo plazo a tiempo (y hay suspended-ready), puede agarrar uno de menor prioridad de ready (porque no llego a ready un suspended-ready a tiempo)
  sem_post(&sem_proceso_listo); //libero un grado de multiprogramacion, hay que mover alguno a ready
  sem_post(&sem_bin_procesar_listo);

  return true;
}

// TODO: implementar
bool bloquear_proceso(pcb_t *proceso_actualizado, int tiempo_bloqueo) {

  pcb_t *proceso = desencolar_proceso_en_ejecucion();   // sacar proceso de lista de ejecucion
  proceso_finalizar_rafaga(proceso);  // actualizar estimacion
  proceso->program_counter = proceso_actualizado->program_counter;  // buscar proceso => actualizar pcb
  encolar_proceso_en_bloqueados(proceso, tiempo_bloqueo); // mover proceso a lista de bloqueados => tener en cuenta https://github.com/sisoputnfrba/foro/issues/2559

  return true;
}

// TODO: implementar
bool desalojar_proceso_interrupt(pcb_t *proceso_actualizado) {

  pcb_t *proceso_desencolado_ejecucion = desencolar_proceso_en_ejecucion(); // sacar proceso de lista de ejecucion
  proceso_finalizar_rafaga(proceso_desencolado_ejecucion);   // actualizar estimacion 
  proceso_desencolado_ejecucion->program_counter = proceso_actualizado->program_counter; // buscar proceso => actualizar pcb
  encolar_proceso_en_listos(proceso_desencolado_ejecucion); // mover a ready

  sem_post(&sem_bin_procesar_listo);  // llamara a planificar corto plazo

  return true;
  
}