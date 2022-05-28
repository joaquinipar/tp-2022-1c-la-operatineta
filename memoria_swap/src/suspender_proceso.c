#include "../include/suspender_proceso.h"
bool memoria_suspender_proceso(uint32_t pid)
{return false;}

/*

bool memoria_suspender_proceso(uint32_t pid) {
  
  if(esta_proceso_en_lista_tablas_1er_nivel(pid)==1){
    if(mem_swap_config->algoritmo_reeemplazo == 0){
      //Caso CLOCK - Bajan todas las paginas
       trace_log("memoria_principal.c@memoria_suspender_proceso", "Caso CLOCK - Bajan todas las paginas"); 
       bajar_paginas_swap_clock(pid); 
    }
    if(mem_swap_config->algoritmo_reeemplazo == 1){
      //Caso CLOCK Modificado- Bajan solo las paginas que fueron modificadas. BM =1
      trace_log("memoria_principal.c@memoria_suspender_proceso", "Caso CLOCK  MOD - Bajan las paginas modificadas BM = 1 "); 
       bajar_paginas_swamp_clockmod(pid); 
    }
      eliminar_reserva_marcos_proceso_MP(pid); 
      format_info_log("memoria_suspender_proceso.c@memoria_suspender_proceso","[SUSPENSION PROCESO] - Proceso: %d", pid); 
      return true; 
  }
    format_error_log("memoria_suspender_proceso.c@memoria_suspender_proceso","[ERROR AL SUSPENDER] - Proceso: %d", pid);
    return false;
}
 */