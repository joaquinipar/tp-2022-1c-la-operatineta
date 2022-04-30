#ifndef MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_
#define MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_

#include <utils/logger.h>
#include <commons/collections/list.h>
#include "configuracion_mem_swap.h"
#include "struct_mem.h"
#include "memoria_principal_helper.h"
#include "memoria_api.h"

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


void iniciar_memoria_principal(); 
mem_principal_t *crear_mem_principal();
void destruir_estructura_mem_principal();
/**
* @NAME: iniciar_listas_globales_de_tablas
* @DESC: Inicia las listas de tablas, tanto de primer nivel como de segundo nivel.
*/
void iniciar_listas_globales_de_tablas();
void inicializar_tablas();
void crear_array_mem();

#endif /* MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_ */
