#ifndef MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_
#define MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_

#include <utils/logger.h>
#include "configuracion_mem_swap.h"
#include <stdint.h>
#include "struct_mem.h"
#include "memoria_principal.h"
#include "swap/swap.h"
#include "memoria_principal_helper.h"

/**
* @NAME: inicio_proceso
* @DESC: Inicializa un proceso creando sus tablas de paginas (1er y 2do nivel) y su archivo de swap. Agrega
 * las tablas y el archivo de swap a sus correspondientes listas. Retorna
*/
uint32_t inicio_proceso(uint32_t pid, uint32_t tamanio);

uint32_t buscar_nro_tabla_2do_nivel (uint32_t pid, uint32_t nro_tabla_1er_nivel, uint32_t nro_entrada_1er_nivel);

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


#endif /* MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_ */
