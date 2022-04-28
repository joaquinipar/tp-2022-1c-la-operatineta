#ifndef MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_
#define MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_

#include <utils/logger.h>
#include "configuracion_mem_swap.h"
#include <stdint.h>
#include "struct_mem.h"
#include "memoria_principal.h"
#include "swap/swap.h"
#include "memoria_principal_helper.h"

uint32_t inicio_proceso(uint32_t pid, uint32_t tamanio);


#endif /* MEMORIA_SWAP_INCLUDE_MEMORIA_API_H_ */
