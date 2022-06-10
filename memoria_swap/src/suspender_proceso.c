#include "../include/suspender_proceso.h"


bool memoria_suspender_proceso(uint32_t pid) {
  
  if(proceso_presente_en_lista_tabla_1er_nivel(pid)){

    format_info_log("memoria_suspender_proceso.c@memoria_suspender_proceso","[SUSPENSION PROCESO] - Proceso: %d", pid); 
    bajar_paginas_swap(pid); 
    return true; 
  }
    format_error_log("memoria_suspender_proceso.c@memoria_suspender_proceso","[ERROR AL SUSPENDER] - Proceso: %d no se encuentra en memoria", pid);
    return false;
}
 