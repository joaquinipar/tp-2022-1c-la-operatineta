#ifndef CPU_INCLUDE_TLB_H_
#define CPU_INCLUDE_TLB_H_

#include <sys/time.h>
#include <stdint.h>
#include <utils/logger.h>
#include "configuracion_cpu.h"

typedef struct {
  // indice del array seran las entradas de la tlb
  //uint32_t id_proceso;
  uint32_t nro_pagina;
  uint32_t marco;
  int estado; // 0 -> libre 1-> ocupado 
  long long  time_pagina;
} tlb_t;

 //--- VAR GLOBABLES --- 
tlb_t* array_tlb;

void iniciar_tlb(); 
void destruir_estructura_TLB(); 
long long get_timestamp();
int pagina_presente_TLB(uint32_t pagina_buscada); 
int buscar_pagina_en_TLB(uint32_t pagina_buscada); 
int cantidad_entradas_tlb_libres(); 
int esta_llena_tlb(); 
int algoritmo_First_Fit_TLB(); 
int get_victima_tlb(); 
int algoritmo_reemplazo_TLB(); 
int escribir_entrada_en_tlb(uint32_t pagina_a_escribir, uint32_t marco_a_escribir); 
void eliminar_entradas_TLB(); 
uint32_t get_marco_de_pagina_TLB(uint32_t pagina_buscada); 
uint32_t se_encuentra_en_tlb(uint32_t pagina_buscada);
void imprimir_estado_array_TLB();






#endif /* CPU_INCLUDE_TLB_H_ */
