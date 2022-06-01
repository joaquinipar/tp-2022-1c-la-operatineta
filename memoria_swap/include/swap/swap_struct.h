#ifndef MEMORIA_SWAP_SWAP_STRUCT_H
#define MEMORIA_SWAP_SWAP_STRUCT_H

#include <stdlib.h>
#include <stdint.h>
#include "../../../static/include/utils/logger.h"

typedef struct{
    // El indice del array representa el numero de marco
    int pagina;
}marco_virtual_t;

typedef struct{
    uint32_t pid;
    char* path_archivo;
    marco_virtual_t* array_marcos_virtual_del_proceso;
    void* area_archivo_swap;
    uint32_t tam_proceso;
}archivo_pid_t;

#endif //MEMORIA_SWAP_SWAP_STRUCT_H
