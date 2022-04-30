#ifndef CPU_INCLUDE_CICLO_DECODE_FETCH_H_
#define CPU_INCLUDE_CICLO_DECODE_FETCH_H_

#include "configuracion_cpu.h"
#include <stdint.h>
#include <utils/logger.h>
#include "struct_cpu.h"
//#include <commons/collections/list.h>



typedef struct {

	int32_t instruccion;
	int32_t argumentos[2];

}instruccion_t;



int is_copy_instruction(instruccion_t*);
void fetch_instruction(pcb_t*);
void fetch_operand(instruccion_t*);
instruccion_t* decode(pcb_t*);





#endif /* CPU_INCLUDE_CICLO_DECODE_FETCH_H_ */
