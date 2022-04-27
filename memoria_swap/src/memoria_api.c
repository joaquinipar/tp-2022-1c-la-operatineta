#include "../include/memoria_api.h"

uint32_t inicio_proceso(uint32_t pid, uint32_t tamanio){
    info_log("memoria_api.c@inicio_struct_adm", "Se inicia las estructuras administrativas del proceso"); 
    
    archivo_pid_t* un_arch_swap = crear_archivo_pid(pid, tamanio);
    list_add(list_archivos_swap, un_arch_swap); 

    uint32_t cant_paginas_a_utilizar = obtener_cant_paginas_a_utilizar(tamanio);
    uint32_t cant_tablas_2do_nivel = cant_paginas_a_utilizar % mem_swap_config->entradas_por_tabla;

    uint32_t cant_entradas_a_usar_1er_nivel = cant_tablas_2do_nivel; 
    tabla_t* una_tabla_1er_nivel = malloc(sizeof(tabla_t));
    una_tabla_1er_nivel->pid = pid; 
    una_tabla_1er_nivel->tabla_pagina_1er_nivel = list_create();


    una_tabla_1er_nivel->tabla_pagina_2do_nivel = list_create();
    
    for(int i=0;i<cant_tablas_2do_nivel;i++){
    tabla_2do_nivel_t* una_tabla_2do_nivel = malloc(sizeof(tabla_2do_nivel_t));
    una_tabla_2do_nivel->contenido_tabla_2do_nivel =malloc(sizeof(pagina_2do_nivel_t)*mem_swap_config->entradas_por_tabla);
    list_add(una_tabla_1er_nivel->tabla_pagina_2do_nivel, una_tabla_2do_nivel); 
    list_add(una_tabla_1er_nivel->tabla_pagina_1er_nivel,i); 
    }

    list_add(list_tablas_1er_nivel,una_tabla_1er_nivel); 

    //TODO agregar las paginas a utilizar con los marcos libres condicional si esta lleno o no bla bla bla 

    
                           
   
   
   
   
  
    return value_table; 
} 

int32_t obtener_cant_paginas_a_utilizar(int32_t tamanio_total_size) {

  int32_t cant_total_page = ceil((float)tamanio_total_size / (float)mem_swap_config->tamanio_pagina);
  format_debug_log(".c@obtener_cant_paginas_a_utilizar", "La cantidad de paginas a utilizar son: %d", cant_total_page);
  return cant_total_page;
}

