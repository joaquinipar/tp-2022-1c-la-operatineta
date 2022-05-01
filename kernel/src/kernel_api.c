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

  /*agregar_proceso_a_lista_de_procesos(proceso);

  if (encolar_proceso_en_nuevos(proceso)) {
    return true;
  }
*/
  return true;
}


//bool cerrar_proceso(uint32_t pid);
