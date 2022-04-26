#ifndef CPU_INCLUDE_TLB_H_
#define CPU_INCLUDE_TLB_H_

#include <stdint.h>
#include <utils/logger.h>
#include "configuracion_cpu.h"

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

#endif /* CPU_INCLUDE_TLB_H_ */
