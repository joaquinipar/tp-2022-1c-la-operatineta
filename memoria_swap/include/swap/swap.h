#ifndef MEMORIA_SWAP_SWAP_H
#define MEMORIA_SWAP_SWAP_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <commons/collections/list.h>
#include "../configuracion_mem_swap.h"
#include "swap_struct.h"
#include "swap_helper.h"

/**
* @NAME: crear_archivo_pid_t
* @DESC: Crea un archivo_pid_t con sus campos rellenados. Esta función también crea el archivo de swap del proceso.
*/
t_list* list_archivos_swap;


archivo_pid_t* crear_archivo_pid(int pid, uint32_t tam_proceso);
void iniciar_swap();
void destruir_list_swap(); 

#endif //MEMORIA_SWAP_SWAP_H