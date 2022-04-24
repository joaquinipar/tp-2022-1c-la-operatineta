#ifndef INCLUDE_CPU_STRUCT_H_
#define INCLUDE_CPU_STRUCT_H_

#include <stdlib.h>
#include <stdint.h> 
#include <commons/collections/list.h> 


typedef struct {
  uint32_t pid;
  t_list*  lista_instrucciones;
  uint32_t tamanio;
  uint32_t program_counter;
}pcb_t;












#endif /*INCLUDE_CPU_STRUCT_H_*/