#include "../include/cerrar_proceso.h"

bool memoria_cerrar_proceso(uint32_t pid) {

  if(proceso_presente_en_lista_tabla_1er_nivel(pid)){

    format_info_log("cerrar_proceso.c@memoria_cerrar_proceso","[ELIMINAR PROCESO] - Proceso: %d", pid); 
    for (int marco = 0; marco < mem_ppal->cant_marcos; marco++) {

      if (array_marcos[marco].pid == pid) {
      
        array_marcos[marco].pid = -1; 
        array_marcos[marco].estado = 0; 
        array_marcos[marco].pagina = NULL; 
        
      } 
    }
    format_debug_log("cerrar_proceso.c@memoria_cerrar_proceso", "Se elimina PID %d de la memoria", pid); 

    eliminar_proceso_en_lista_global_swap(pid); 
    
    format_debug_log("cerrar_proceso.c@memoria_cerrar_proceso", "Se elimina PID %d de Swap", pid); 

      bool buscar_por_pid(pcb_t* proceso) {
      return proceso->pid == pid;
      }
    list_remove_by_condition(lista_tablas_1er_nivel, (void *)buscar_por_pid);
    list_remove_by_condition(lista_tablas_2do_nivel, (void *)buscar_por_pid);
    format_debug_log("cerrar_proceso.c@memoria_cerrar_proceso", "Se elimina PID %d de la listas globales de 1er nivel y 2do nivel", pid); 
    format_info_log("cerrar_proceso.c@memoria_cerrar_proceso","[ELIMINACION EXITOSA] - Proceso: %d", pid); 
    return true; 

  }
    format_error_log("cerrar_proceso.c@memoria_cerrar_proceso","[ERROR AL ELIMINAR PROCESO] - Proceso: %d no se encuentra en memoria", pid);
    return false;
}

