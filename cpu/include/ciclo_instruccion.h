#ifndef CPU_INCLUDE_CICLO_INSTRUCCION_H_
#define CPU_INCLUDE_CICLO_INSTRUCCION_H_

#include "configuracion_cpu.h"
#include <utils/logger.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/structs.h>
#include <commons/collections/list.h>
#include "conexion_client.h"
#include "server_dispatch.h"
#include "mmu.h"
#include "tlb.h"

void  iniciar_ciclo_instruccion(pcb_t* proceso); 
instruccion_t* fetch_instruction(pcb_t* proceso); 
int32_t gestionar_instruccion_read(pcb_t* proceso, int32_t direccion_logica); 
void gestionar_instruccion_write(pcb_t* proceso, int32_t direccion_logica, int32_t valor_a_copiar); 
void gestionar_instruccion_copy(pcb_t* proceso, instruccion_t* instruccion); 
pcb_t* execute_instruction(instruccion_t* instruccion_a_ejecutar, pcb_t* proceso);
bool requiere_desalojo(instruccion_t* instruccion); 
bool hay_interrupcion(pcb_t * proceso); 
void destruir_proceso(pcb_t* proceso_actualizado);

void encender_flag_interrupt(); 
void apagar_flag_interrupt();
bool chequear_interrupt(); 


//--------------
void print_instruccion(instruccion_t* una_instruccion); 

#endif /* CPU_INCLUDE_CICLO_INSTRUCCION_H_ */
