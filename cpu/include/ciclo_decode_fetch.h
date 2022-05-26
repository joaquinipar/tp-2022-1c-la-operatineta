#ifndef CPU_INCLUDE_CICLO_DECODE_FETCH_H_
#define CPU_INCLUDE_CICLO_DECODE_FETCH_H_

#include "configuracion_cpu.h"
#include <stdint.h>
#include <unistd.h>
#include <utils/logger.h>
#include "struct_cpu.h"
#include <utils/protocolo_comunicacion.h>
#include <cpu_api.h>
//#include <commons/collections/list.h>




int is_copy_instruction(instruccion_t*);
void inicializar_interruption_flag();
int hay_interrupcion();
instruccion_t* fetch_instruction(pcb_t*);
void fetch_operand(instruccion_t*);
uint32_t execute_instruction(instruccion_t*);
//instruccion_t* decode(pcb_t*);







#endif /* CPU_INCLUDE_CICLO_DECODE_FETCH_H_ */
