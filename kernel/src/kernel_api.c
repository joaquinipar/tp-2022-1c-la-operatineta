#include "../include/kernel_api.h"

bool admitir_proceso_nuevo(uint32_t pid, int socket) {

  char *mensaje = string_from_format("Admitiendo proceso nuevo con id: %d al sistema", pid);
  debug_log("kernel/kernel_api@admitir_proceso_nuevo", mensaje);
  free(mensaje);

  //pcb_t* proceso = crear_proceso(pid, socket, socket_memoria);

  /*agregar_proceso_a_lista_de_procesos(proceso);

  if (encolar_proceso_en_nuevos(proceso)) {
    return true;
  }
*/
  return false;
}


//bool cerrar_proceso(uint32_t pid);
