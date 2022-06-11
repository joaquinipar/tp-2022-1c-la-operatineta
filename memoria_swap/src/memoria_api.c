#include "../include/memoria_api.h"

uint32_t admitir_proceso(uint32_t pid, uint32_t tamanio){

    format_info_log("memoria_api.c@admitir_proceso", "Comienza admision del proceso en Memoria - Pid: %d -- Tamaño: %d", pid, tamanio); 
    uint32_t posicion_tabla_1er_nivel_en_lista_global = inicio_proceso(pid, tamanio); 

    if(posicion_tabla_1er_nivel_en_lista_global==-1){
        format_error_log("proceso.c@admitir_proceso", "[ERROR] - Proceso %d ya admitido en Memoria");
    }



    return posicion_tabla_1er_nivel_en_lista_global; 
} 


bool suspender_proceso(uint32_t pid) {
  format_info_log("memoria_api.c@suspender_proceso", "Comienza SUSPENDER_PROCESO - Proceso: %d", pid);
  return memoria_suspender_proceso(pid);
}

bool cerrar_proceso(uint32_t pid) {
  format_info_log("memoria_api.c@cerrar_proceso", "Comienza CERRAR_PROCESO - Proceso: %d", pid);
  return memoria_cerrar_proceso(pid);;
}