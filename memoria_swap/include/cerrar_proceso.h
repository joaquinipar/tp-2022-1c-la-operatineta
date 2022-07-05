#ifndef MEMORIA_SWAP_INCLUDE_CERRAR_PROCESO_H_
#define MEMORIA_SWAP_INCLUDE_CERRAR_PROCESO_H_

#include <stdbool.h>
#include <utils/logger.h>
#include <utils/structs.h>
#include "configuracion_mem_swap.h"
#include "struct_mem.h"
#include "swap/swap.h"
#include "memoria_principal.h"


bool memoria_cerrar_proceso(uint32_t pid); 


#endif /* MEMORIA_SWAP_INCLUDE_CERRAR_PROCESO_H_ */
