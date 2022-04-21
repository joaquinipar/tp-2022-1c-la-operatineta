#ifndef INCLUDE_SWAP_STRUCT_H_
#define INCLUDE_SWAP_STRUCT_H_

#include <stdlib.h>
#include <stdint.h> 
#include "../../../static/include/utils/logger.h"

typedef struct{
	// El indice del array representa el numero de marco
	int estado;
	int pagina;
}marco_virtual_t;

typedef struct{
    uint32_t pid;
    char* path_archivo;
    marco_virtual_t* array_marcos_virtual_del_proceso;
    void* area_archivo_swap; 	
}archivo_pid_t;

#endif /*INCLUDE_SWAP_STRUCT_H_*/