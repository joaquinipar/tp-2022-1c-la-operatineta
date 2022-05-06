#ifndef CPU_INCLUDE_MMU_H_
#define CPU_INCLUDE_MMU_H_

#include <stdint.h>
#include <utils/logger.h>
#include <math.h>
#include "configuracion_cpu.h"
#include "conexion_client.h"


uint32_t obtener_pagina(uint32_t direccion_logica); 
uint32_t obtener_entrada_tabla_1er_nivel(uint32_t numero_pagina); 
uint32_t obtener_entrada_tabla_2do_nivel(uint32_t numero_pagina); 
uint32_t obtener_desplazamiento(uint32_t direccion_logica, uint32_t numero_pagina); 

#endif /* CPU_INCLUDE_MMU_H_ */
