#ifndef MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_
#define MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_

#include <utils/logger.h>
#include "configuracion_mem_swap.h"
#include <stdint.h>
#include "struct_mem.h"
#include "memoria_principal.h"
#include "proceso.h"
#include "swap/swap.h"
#include "cerrar_proceso.h"
#include "suspender_proceso.h"

/**
* @NAME: admitir_proceso
* @DESC: Admite un proceso nuevo en memoria. Se le crean las estructuras. 
*/
uint32_t admitir_proceso(uint32_t pid, uint32_t tamanio);

bool suspender_proceso(uint32_t pid); 
bool cerrar_proceso(uint32_t pid); 










#endif /* MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_ */
