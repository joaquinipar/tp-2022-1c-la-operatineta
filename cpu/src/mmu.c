#include "../include/mmu.h"

uint32_t obtener_pagina(uint32_t direccion_logica) {
  uint32_t nro_pagina = floor(direccion_logica / datos_traduccion->tamanio_pagina);
  format_debug_log("mmu.c@obtener_pagina", "DL: %d - Pagina correspondiente: %d", direccion_logica, nro_pagina); 
  return nro_pagina;
}

uint32_t obtener_entrada_tabla_1er_nivel(uint32_t numero_pagina){
 uint32_t entrada_tabla_1er_nivel = floor(numero_pagina / datos_traduccion->cantidad_entradas_tabla);
 format_debug_log("mmu.c@obtener_pagina", "Pagina: %d - Entrada 1er nivel:%d", numero_pagina, entrada_tabla_1er_nivel); 
 return entrada_tabla_1er_nivel; 
}


uint32_t obtener_entrada_tabla_2do_nivel(uint32_t numero_pagina){
 uint32_t entrada_tabla_2do_nivel = numero_pagina % (datos_traduccion->cantidad_entradas_tabla); 
 format_debug_log("mmu.c@obtener_pagina", "Pagina: %d - Entrada 2do nivel:%d", numero_pagina, entrada_tabla_2do_nivel); 
 return entrada_tabla_2do_nivel; 
}

uint32_t obtener_desplazamiento(uint32_t direccion_logica, uint32_t numero_pagina){

  uint32_t desplazamiento = direccion_logica - numero_pagina * datos_traduccion->tamanio_pagina; 
  format_debug_log("mmu.c@obtener_pagina", "DL: %d - Pagina:%d - Offset",direccion_logica, numero_pagina, desplazamiento); 
  return desplazamiento; 
}

int esta_dentro_espacio_direcciones(pcb_t* un_proceso, uint32_t direccion_logica){
    if(direccion_logica <= un_proceso->tamanio){
        info_log("memoria_principal_helper.c@esta_dentro_espacio_direcciones", "DL OK");
        return 1; 
    }
    error_log("memoria_principal_helper.c@esta_dentro_espacio_direcciones", "DL fuera del espacio de direcciones");
    return 0; 
}
