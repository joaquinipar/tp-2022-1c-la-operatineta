#ifndef CPU_INCLUDE_CPU_API_H_
#define CPU_INCLUDE_CPU_API_H_

#include <utils/logger.h>
#include <utils/protocolo_comunicacion.h>
#include <commons/collections/list.h>
#include "configuracion_cpu.h"
#include <utils/structs.h>
#include "configuracion_cpu.h"
#include "ciclo_instruccion.h"

/*
op_code_t ciclo_de_instruccion(pcb_t*);
op_code_t procesar_solicitud_de_kernel(pcb_t*);
void setear_variable_de_interrupciones(uint32_t);
void setear_variable_de_IO(uint32_t);
void inicializar_variables_globales();
int requiere_desalojo(op_code_t CODOP);
int hay_interrupcion();

uint32_t INTERRUPTION_FLAG;
uint32_t I_O_FLAG;
*/


void iniciar_modulo(pcb_t* proceso_recibido); 




#endif /* CPU_INCLUDE_CPU_API_H_ */
