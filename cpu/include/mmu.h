#ifndef CPU_INCLUDE_MMU_H_
#define CPU_INCLUDE_MMU_H_

#include <stdint.h>
#include <math.h>
#include <utils/logger.h>
#include <math.h>
#include "configuracion_cpu.h"
#include "conexion_client.h"
#include <utils/structs.h>


uint32_t obtener_pagina(uint32_t direccion_logica); 
uint32_t obtener_entrada_tabla_1er_nivel(uint32_t numero_pagina); 
uint32_t obtener_entrada_tabla_2do_nivel(uint32_t numero_pagina); 
uint32_t obtener_desplazamiento(uint32_t direccion_logica, uint32_t numero_pagina); 
uint32_t obtener_direccion_fisica(uint32_t marco_asignado, uint32_t desplazamiento); 
int esta_dentro_espacio_direcciones(pcb_t* un_proceso, uint32_t direccion_logica); 

#endif /* CPU_INCLUDE_MMU_H_ */
