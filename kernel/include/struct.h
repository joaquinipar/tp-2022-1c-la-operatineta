#ifndef KERNEL_INCLUDE_STRUCT_H_
#define KERNEL_INCLUDE_STRUCT_H_

#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <utils/logger.h>

typedef enum estado_proceso_t {
  NEW,
  READY,
  EXEC,
  BLOCKED,
  BLOCKED_SUSPENDED,
  SUSPENDED_READY,
  EXIT
} estado_proceso_t;

typedef struct {
  uint32_t pid;
  t_list*  lista_instrucciones;
  uint32_t tamanio;
  uint32_t program_counter;
  uint32_t tabla_paginas;
  uint32_t estimacion_rafaga;
  estado_proceso_t estado; 
  uint32_t socket; 

}pcb_t;


#endif /* KERNEL_INCLUDE_STRUCT_H_ */
