#ifndef CPU_INCLUDE_CPU_API_H_
#define CPU_INCLUDE_CPU_API_H_

#include <utils/logger.h>
#include <utils/protocolo_comunicacion.h>
#include <commons/collections/list.h>
#include "configuracion_cpu.h"
#include "struct_cpu.h"
#include "configuracion_cpu.h"
#include "ciclo_decode_fetch.h"

op_code_t ciclo_de_instruccion(pcb_t*);
void procesar_solicitud_de_kernel(pcb_t*);
void setear_variable_de_interrupciones(int);
int hay_interrupcion();

uint32_t INTERRUPTION_FLAG;




#endif /* CPU_INCLUDE_CPU_API_H_ */
