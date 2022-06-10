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

  format_info_log("kernel/kernel_api@ejecutar_proceso_nuevo", "Admitiendo proceso nuevo con id: %d al sistema", proceso->pid);
  format_debug_log("kernel/kernel_api@ejecutar_proceso_nuevo", "Tamaño de proceso: %d ", proceso->tamanio);

  agregar_proceso_a_lista_de_procesos(proceso);

  return encolar_proceso_en_nuevos(proceso);

}

bool finalizar_proceso(pcb_t *proceso_actualizado) {
  format_info_log("kernel_api.c@finalizar_proceso", "Instruccion EXIT recibida para proceso con id: %d", proceso_actualizado->pid);

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
  //sem_post(&sem_proceso_listo); //libero un grado de multiprogramacion, hay que mover alguno a ready
  // no realmente, el semaforo que esta abajo va a dar luz verde para que pase un nuevo proceso si es que hay
  // hacer este sem_post es obligar a pasar a un nuevo proceso a ready cuando este no existe y boom. 
  sem_post(&sem_grado_multiprogramacion_disponible);
  sem_post(&sem_bin_procesar_listo);

  return true;
}

bool bloquear_proceso(pcb_t *proceso_actualizado, int tiempo_bloqueo) {

  pcb_t *proceso = desencolar_proceso_en_ejecucion();   // sacar proceso de lista de ejecucion
  proceso_finalizar_rafaga(proceso);  // actualizar estimacion
  proceso->program_counter = proceso_actualizado->program_counter;  // buscar proceso => actualizar pcb
  encolar_proceso_en_bloqueados(proceso, tiempo_bloqueo); // mover proceso a lista de bloqueados, guardar el tiempo de IO
  lanzar_thread_suspension_proceso(proceso); // despues del tiempo maximo de bloqueo lo suspende si sigue bloqueado y manda el mensaje a memoria
  incrementar_cantidad_procesos_bloqueados();
  //sem_post(&sem_grado_multiprogramacion_disponible);
  sem_post(&sem_bin_procesar_listo); // como se libera la cpu se puede ejecutar otro proceso, si es que hay.
  return true;

}

// TODO: implementar
bool desalojar_proceso_interrupt(pcb_t *proceso_actualizado) {

  pcb_t *proceso_desencolado_ejecucion = desencolar_proceso_en_ejecucion(); // sacar proceso de lista de ejecucion
  proceso_actualizar_rafaga_por_desalojo(proceso_desencolado_ejecucion);   // actualizar estimacion por desalojo 
  proceso_desencolado_ejecucion->program_counter = proceso_actualizado->program_counter; // buscar proceso => actualizar pcb
  encolar_proceso_en_listos(proceso_desencolado_ejecucion); // mover a ready

  sem_post(&sem_bin_procesar_listo);  // llamara a planificar corto plazo

  return true;
  
}