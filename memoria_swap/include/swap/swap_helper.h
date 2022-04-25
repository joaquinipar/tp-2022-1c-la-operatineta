#ifndef MEMORIA_SWAP_SWAP_HELPER_H
#define MEMORIA_SWAP_SWAP_HELPER_H

#include <unistd.h>
#include <sys/types.h>
#include <commons/string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "../configuracion_mem_swap.h"
#include "swap_struct.h"

/**
* @NAME: obtener_ruta_proceso
* @DESC: Crea un string con la ruta del archivo de swap del proceso.
*/
char* obtener_ruta_swap_proceso(int pid);

/**
* @NAME: crear_archivo_swap
* @DESC: Crea el archivo de swap en la dirección establecida en el archivo. Mapea el archivo a memoria para poder accederlo
 * mediante un void*
*/
int crear_archivo_swap(archivo_pid_t* archivo);

#endif //MEMORIA_SWAP_SWAP_HELPER_H
