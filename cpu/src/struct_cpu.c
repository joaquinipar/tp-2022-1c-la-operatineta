#include "../include/struct_cpu.h"

typedef struct {
  uint32_t pid;
  t_list*  lista_instrucciones;
  uint32_t tamanio;
  uint32_t program_counter;
}pcb_t;
