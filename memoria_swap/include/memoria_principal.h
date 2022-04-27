#ifndef MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_
#define MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_

#include <utils/logger.h>
#include <commons/collections/list.h>
#include "configuracion_mem_swap.h"
#include "struct_mem.h"



void iniciar_memoria_principal(); 
mem_principal_t *crear_mem_principal();
void destruir_estructura_mem_principal();

#endif /* MEMORIA_SWAP_INCLUDE_MEMORIA_PRINCIPAL_H_ */
