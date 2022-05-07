#include "../include/tlb.h"


void iniciar_tlb() {
  info_log("tlb.c@iniciar_tlb", "Se inicializa TLB vacia"); 
  array_tlb = NULL;

  array_tlb = (tlb_t *)malloc(cpu_config->entradas_tlb * sizeof(tlb_t));
  if (array_tlb == NULL) {
    error_log("tlb.c@iniciar_tlb", "ACa estamos con un hermoso error iniciando la TLB");
  }

  int entrada;
  for (entrada = 0; entrada < cpu_config->entradas_tlb; ++entrada) {
    array_tlb[entrada].estado = 0;
    array_tlb[entrada].nro_pagina = -1;
    array_tlb[entrada].marco = -1;
    array_tlb[entrada].time_pagina = -1; /// Seteo todo en -1 por las dudas.
  }

  debug_log("tlb.c@iniciar_tlb", "Finaliza la inicializacion la TLB vacia");
}
void destruir_estructura_TLB() {
  info_log("tlb.c@iniciar_tlb", "Se destruye estructura TLB"); 
  free(array_tlb);
}

long long get_timestamp() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long long milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
  // printf("milliseconds: %lld\n", milliseconds);
  return milliseconds;
} 

int pagina_presente_TLB(uint32_t pagina_buscada) {
  int entrada;
  for (entrada = 0; entrada < cpu_config->entradas_tlb; entrada++) {

    if ((array_tlb[entrada].nro_pagina == pagina_buscada)) {
      format_debug_log("tlb.c@pagina_presente_TLB","Pagina: %d -- Se encuentra en la TLB", pagina_buscada);
      return 1; 
    }
  }
  format_error_log("tlb.c@pagina_presente_TLB",  "[ERROR] - NO se encuentra Pagina: %d en la TLB",pagina_buscada);
  return 0;
}

int buscar_pagina_en_TLB(uint32_t pagina_buscada) {

  if (pagina_presente_TLB(pagina_buscada) == 1) {
    int entrada;
    for (entrada = 0; entrada < cpu_config->entradas_tlb; entrada++) {

      if (array_tlb[entrada].nro_pagina == pagina_buscada) {
        format_debug_log("tlb.c@buscar_pagina_en_TLB", "Pagina presente en TLB: %d", pagina_buscada);
        return entrada;
      }
    }
  }
  error_log("tlb.c@buscar_pagina_en_TLB", "[ERROR] - No se encuentra pagina en TLB");
  return -1;
}

int cantidad_entradas_tlb_libres() {

  int entrada;
  int cantidad_entradas_tlb_libres = 0;
  for (entrada = 0; entrada < cpu_config->entradas_tlb; entrada++) {

    if ((array_tlb[entrada].estado == 0) && (array_tlb[entrada].nro_pagina == -1)) {
      cantidad_entradas_tlb_libres++;
    }
  }
  format_debug_log("tlb.c@cantidad_entradas_tlb_libres","Cantidad Entradas Libres:%d", cantidad_entradas_tlb_libres);
  return cantidad_entradas_tlb_libres;
}

int esta_llena_tlb() {
  if (cantidad_entradas_tlb_libres() >= 1) {
    debug_log("tlb.c@esta_llena_tlb", "Hay entradas libres en la TLB");
    return 1;
  } else {
    error_log("tlb.c@esta_llena_tlb", "[ERROR] - TLB LLENA");
    return 0;
  }
}

int algoritmo_First_Fit_TLB() {

  int entrada;
  int entrada_libre = -1;

  for (entrada = 0; entrada < cpu_config->entradas_tlb;entrada++) {
    if ((array_tlb[entrada].estado == 0) && (array_tlb[entrada].nro_pagina == -1)) {
      entrada_libre = entrada;
      break;
    }
  }
  format_debug_log("tlb.c@algoritmo_First_Fit_TLB", "Primer entrada libre: %d", entrada_libre);
  return entrada_libre;
}


int get_victima_tlb(){

	int entrada_record = 0;
	long long  entrada_record_time = array_tlb[0].time_pagina;

  for(int i=0; i < cpu_config->entradas_tlb; i++){

	  if(entrada_record_time > array_tlb[i].time_pagina){

		  entrada_record = i;
		  entrada_record_time = array_tlb[i].time_pagina;
	  }

  }

  return entrada_record;

}
int algoritmo_reemplazo_TLB(){

  if(cpu_config->reemplazo_tlb == 0){
    info_log("tlb.c@algoritmo_reemplazo_TLB","Algoritmo TLB asignado es: FIFO"); 
  }
  info_log("tlb.c@algoritmo_reemplazo_TLB","Algoritmo TLB asignado es: LRU"); 

	  int victim = get_victima_tlb();

	  if(victim == -1){ // Caso no se encontro victima (rarisimo)
      error_log("tlb.c@algoritmo_reemplazo_TLB", "NO se encuentra victima");      
      return -1; 
      } 

return victim; 
}



int escribir_entrada_en_tlb(uint32_t pagina_a_escribir, uint32_t marco_a_escribir) {

  if (esta_llena_tlb() == 1) { // Entra si esta vacío xD!
    int entrada_a_escribir = algoritmo_First_Fit_TLB();

    array_tlb[entrada_a_escribir].nro_pagina = pagina_a_escribir;
    array_tlb[entrada_a_escribir].marco = marco_a_escribir;
    array_tlb[entrada_a_escribir].estado = 1;

    long long time_tlb = get_timestamp();
    array_tlb[entrada_a_escribir].time_pagina = time_tlb;

    format_trace_log("tlb.c@escribir_entrada_en_tlb", "[ESCRITURA] Entrada: %d - Pagina: %d - Marco: %d - Time: %lld -Estado: %d", entrada_a_escribir, array_tlb[entrada_a_escribir].nro_pagina, array_tlb[entrada_a_escribir].marco, (long long)array_tlb[entrada_a_escribir].time_pagina, array_tlb[entrada_a_escribir].estado);
    return 1;
  }
  format_warning_log("tlb.c@escribir_entrada_en_tlb", "TLB LLena - Vamos a reemplazar ... ");
  int entrada_victima = algoritmo_reemplazo_TLB(); 
  format_debug_log("tlb.c@escribir_entrada_en_tlb","Entrada victima: %d",  entrada_victima);
  return 0;
}

int borrar_pagina_TLB(uint32_t pagina_buscada) {

  int entrada_buscada = buscar_pagina_en_TLB(pagina_buscada);
  if (entrada_buscada != -1) {
    array_tlb[entrada_buscada].nro_pagina = -1;
    array_tlb[entrada_buscada].marco = -1;
    array_tlb[entrada_buscada].estado = 0;
    array_tlb[entrada_buscada].time_pagina = -1;
    format_debug_log("tlb.c@borrar_pagina_TLB","Delete Entrada : %dPagina: %d", entrada_buscada, pagina_buscada);
    return 1;
  }

  format_error_log("tlb.c@borrar_pagina_TLB", "No se puede borrar Entrada Victima: %d - Pagina: %d", entrada_buscada, pagina_buscada);
  return 0;
}

void eliminar_entradas_TLB() {

  int entrada;
  for (entrada = 0; entrada < cpu_config->entradas_tlb; entrada++) {
      array_tlb[entrada].nro_pagina = -1;
      array_tlb[entrada].marco = -1;
      array_tlb[entrada].estado = 0;
      array_tlb[entrada].time_pagina = -1;
    }
  debug_log("tlc.c@eliminar_entradas_TLB", "Flush TLB"); 
}
 

uint32_t get_marco_de_pagina_TLB(uint32_t pagina_buscada) {

  int entrada_buscada = buscar_pagina_en_TLB(pagina_buscada);

  if (entrada_buscada != -1) {

    uint32_t nro_marco = array_tlb[entrada_buscada].marco;
    format_trace_log("tlb.c@get_marco_de_pagina_TLB", "Decime que -> Algoritmo TLB: %s", cpu_config->reemplazo_tlb);

    if (cpu_config->reemplazo_tlb == 1) {
      array_tlb[entrada_buscada].time_pagina = get_timestamp();
    }
    
    format_debug_log("tlb.c@get_marco_de_pagina_TLB", "Entrada: %d - Marco Asignado:%d - Pagina %d  -- Time: %lld", entrada_buscada, nro_marco, pagina_buscada,(array_tlb[entrada_buscada].time_pagina));
    return nro_marco;
  }
  return -1;
}


uint32_t rutina_tlb(uint32_t pagina_buscada) {

  //1-Conocer si la pagina del proceso esta en la tlb
  int resultado_presencia_tlb = pagina_presente_TLB(pagina_buscada);

  if (resultado_presencia_tlb == 1) { //HIT
    
    uint32_t marco_asignado = get_marco_de_pagina_TLB(pagina_buscada);
    format_info_log("tlb.c@get_marco_de_pagina_TLB", "Marco Encontrado TLB: %d -- Pagina: %d", marco_asignado, pagina_buscada);
    return marco_asignado;
  }
  //MISS
  error_log("tlb.c@get_marco_de_pagina_TLB", "Marco NO ta en TLB"); 
  return -1;
}
