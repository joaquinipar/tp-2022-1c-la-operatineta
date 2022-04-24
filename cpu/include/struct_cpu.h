#ifndef CPU_INCLUDE_STRUCT_CPU_H_
#define CPU_INCLUDE_STRUCT_CPU_H_

#include <stdlib.h>
#include <stdint.h> 
#include <commons/collections/list.h> 
#include <utils/logger.h>

typedef struct {
  uint32_t pid;
  t_list*  lista_instrucciones;
  uint32_t tamanio;
  uint32_t program_counter;
}pcb_t;



#endif /* CPU_INCLUDE_STRUCT_CPU_H_ */
