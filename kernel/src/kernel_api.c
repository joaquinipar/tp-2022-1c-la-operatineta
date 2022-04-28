#include "../include/kernel_api.h"

pid_t pid_count = 0;

bool admitir_proceso_nuevo(int socket) {

  pid_t pid = ++pid_count;

  format_debug_log("kernel/kernel_api@admitir_proceso_nuevo", "Admitiendo proceso nuevo con id: %d al sistema", pid);

  //pcb_t* proceso = crear_proceso(pid, socket, socket_memoria);

  /*agregar_proceso_a_lista_de_procesos(proceso);

  if (encolar_proceso_en_nuevos(proceso)) {
    return true;
  }
*/
  return false;
}


//bool cerrar_proceso(uint32_t pid);
