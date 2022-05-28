#ifndef MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_
#define MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_

#include <utils/logger.h>
#include <commons/collections/list.h>
#include "configuracion_mem_swap.h"
#include "struct_mem.h"
#include "memoria_api.h"
#include "swap/swap.h"
#include "swap/swap_struct.h"

/*
 * Es la lista de tablas de primer nivel de todos los procesos.
 * Recordemos que cada proceso solo tiene una tabla de primer nivel.
 * Dentro de esta lista, vamos a agregar únicamente elementos de tipo entrada_1er_nivel_t*
 */
t_list* lista_tablas_1er_nivel;


/*
 * Es la lista de tablas de segundo nivel de todos los procesos.
 * Un proceso puede tener mas de una tabla de segundo nivel.
 * Dentro de esta lista, vamos a agregar únicamente elementos de tipo tabla_2do_nivel_t*
 */
t_list* lista_tablas_2do_nivel;

void iniciar_memoria(); 
void iniciar_memoria_principal(); 
mem_principal_t *crear_mem_principal();
void destruir_estructura_mem_principal();
void crear_array_mem(); 
void destruir_array_mem(); 
/**
* @NAME: iniciar_listas_globales_de_tablas
* @DESC: Inicia las listas de tablas, tanto de primer nivel como de segundo nivel.
*/
void iniciar_listas_globales_de_tablas();
void destruir_listas_globales_de_tablas(); 


/**
 * @name leer
 * @desc Lee en la posición de la dirección física (sería el offset que le vamos a agregar al void* de memoria).
 * ATENCIÓN: Esta función devuelve un contenido malloqueado y debe ser liberado posteriormente
 * @param direccion_fisica
 * @return void* (uint32_t)
 */
void* leer(uint32_t direccion_fisica); 

/**
 * @name escribir
 * @desc Escribe N bytes (sizeof(uint32_t)) en la posición indicada por la dirección fisica en la memoria principal.
 * @param direccion_fisica
 * @param contenido
 */
void escribir(uint32_t direccion_fisica, void* contenido); 


void* leer_pagina_memoria(uint32_t marco_asignado); 
int escribir_contenido_swap(void* contenido_marco, uint32_t pid, uint32_t marco); 


void escribir_contenido_en_marco_MP(uint32_t new_pid, pagina_2do_nivel_t *new_page, uint32_t frame_a_escribir, int new_state); 
int marco_presente_MP(uint32_t pagina_buscada, uint32_t pid); 
int buscar_marco_en_MP(uint32_t pagina_buscada, uint32_t pid); 
uint32_t get_marco_MP(uint32_t pagina_buscada, uint32_t pid_buscado); 
int calcular_marcos_ocupados_por_proceso(uint32_t pid); 
bool tiene_marcos_reservados_libres(uint32_t pid); 

#endif /* MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_ */
