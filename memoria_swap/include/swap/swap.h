#ifndef MEMORIA_SWAP_SWAP_H
#define MEMORIA_SWAP_SWAP_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../configuracion_mem_swap.h"
#include "swap_struct.h"
#include "swap_helper.h"

    /**
    * @NAME: crear_archivo_pid_t
    * @DESC: Crea un archivo_pid_t con sus campos rellenados. Esta función también crea el archivo de swap del proceso.
    */
    archivo_pid_t* crear_archivo_pid_t(int pid, uint32_t tam_proceso);

#endif //MEMORIA_SWAP_SWAP_H
