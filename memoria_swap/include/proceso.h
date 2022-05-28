#ifndef MEMORIA_SWAP_INCLUDE_PROCESO_H_
#define MEMORIA_SWAP_INCLUDE_PROCESO_H_

#include <math.h>
#include <utils/logger.h>
#include "configuracion_mem_swap.h"
#include "struct_mem.h"
#include "swap/swap.h"
#include "memoria_principal.h"



uint32_t inicio_proceso(uint32_t pid, uint32_t tamanio); 
/**
* @NAME: inicializar_tabla_1er_nivel
* @DESC: Inicializa las entradas de una tabla de primer nivel. Le asigna el pid y pone las tablas de 2do nivel en -1
*/
entrada_1er_nivel_t*  iniciar_tabla_1er_nivel(uint32_t pid);

/**
* @NAME: buscar_proceso_en_lista_tablas_1er_nivel
* @DESC: Busca la tabla de un proceso en la lista de tablas de 1er nivel. Retorna 1 si lo encontro, 0 si no lo encontro.
*/
int proceso_presente_en_lista_tabla_1er_nivel(uint32_t pid); 

/**
* @NAME: buscar_tabla_1er_nivel_de_un_proceso
* @DESC: Busca la tabla de 1er nivel de un proceso en la lista de tablas de primer nivel.
 * ATENCIÓN: No se debe liberar lo que retorna esta función, ya que está en una lista.
*/
entrada_1er_nivel_t* buscar_tabla_1er_nivel_de_un_proceso(uint32_t pid);
uint32_t buscar_nro_tabla_2do_nivel (uint32_t pid, uint32_t nro_tabla_1er_nivel, uint32_t nro_entrada_1er_nivel);
int32_t obtener_cant_paginas_a_utilizar(int32_t tamanio_total_size); 
tabla_2do_nivel_t* crear_entradas_tabla_2do_nivel(uint32_t pid);
int agregar_entrada_tabla_1er_nivel(entrada_1er_nivel_t* tabla_1er_nivel, uint32_t nro_tabla_2do_nivel); 
void gestionar_tabla_2do_nivel(uint32_t cant_tablas_2do_nivel, entrada_1er_nivel_t* tabla_1er_nivel, uint32_t pid); 

/**
 * @name obtener_marco_de_tabla_2do_nivel
 * @desc Ingresa en la entrada (nro_pagina) de la tabla de segundo nivel (nro_tabla_2do_nivel) y devuelve el marco
 * @param pid
 * @param nro_tabla_2do_nivel
 * @param nro_pagina
 * @return uint32_t
 */
uint32_t obtener_marco_de_tabla_2do_nivel(uint32_t pid, uint32_t nro_tabla_2do_nivel, uint32_t nro_pagina); 

#endif /* MEMORIA_SWAP_INCLUDE_PROCESO_H_ */
