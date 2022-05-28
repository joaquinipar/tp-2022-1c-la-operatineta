#ifndef CPU_INCLUDE_STRUCT_CPU_H_
#define CPU_INCLUDE_STRUCT_CPU_H_

#include <stdlib.h>
#include <stdint.h> 
#include <commons/collections/list.h> 
#include <utils/logger.h>


typedef struct {
	uint32_t cant_entradas_tabla;
	uint32_t tamanio_pagina;
} t_mensaje_pong_memoria;


typedef struct {
  // indice del array seran las entradas de la tlb
  uint32_t id_proceso;
  uint32_t nro_pagina;
  uint32_t marco;
  int estado; // 0 -> libre 1-> ocupado 
  long long  time_pagina;
} tlb_t;

 //--- VAR GLOBABLES --- 
tlb_t* array_tlb;


#endif /* CPU_INCLUDE_STRUCT_CPU_H_ */
