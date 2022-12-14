#ifndef MEMORIA_SWAP_SWAP_H
#define MEMORIA_SWAP_SWAP_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <commons/collections/list.h>
#include "../configuracion_mem_swap.h"
#include "swap_struct.h"
#include "swap_helper.h"
#include "../struct_mem.h"


/**
* @NAME: crear_archivo_pid_t
* @DESC: Crea un archivo_pid_t con sus campos rellenados. Esta función también crea el archivo de swap del proceso.
*/
t_list* list_archivos_swap;


void iniciar_swap();
void destruir_list_swap(); 

archivo_pid_t* crear_archivo_pid(int pid, uint32_t tam_proceso);
uint32_t get_cantidad_marcos(uint32_t tam_proceso);
void admitir_proceso_en_swap(uint32_t pid, uint32_t tamanio); 
int buscar_proceso_swap(uint32_t pid); 
archivo_pid_t* get_proceso_swap(uint32_t pid); 
int eliminar_proceso_en_lista_global_swap(uint32_t pid);
/**
 * @name escribir_pagina_swap
 * @desc Dado un pid y un marco, lo baja a swap
 * @param pid
 * @param marco
 * @return success => 1
 *         error => -1
 */
int escribir_pagina_swap(uint32_t pid, uint32_t pagina);
void* leer_pagina_swap(uint32_t pid, uint32_t pagina);
void eliminar_archivo_directorio_swap(uint32_t);
int obtener_marco_de_pagina_proceso (uint32_t proceso_id, uint32_t pagina, archivo_pid_t* archivo); 
int escribir_contenido_en_swap(uint32_t pid, uint32_t pagina, void* contenido_marco); 

#endif //MEMORIA_SWAP_SWAP_H
