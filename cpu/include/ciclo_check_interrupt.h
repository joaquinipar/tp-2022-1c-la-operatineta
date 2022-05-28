#ifndef CPU_INCLUDE_CICLO_CHECK_INTERRUPT_H_
#define CPU_INCLUDE_CICLO_CHECK_INTERRUPT_H_

#include "configuracion_cpu.h"
#include <utils/logger.h>
 


void encender_flag_interrupt(); 
void apagar_flag_interrupt();
bool chequear_interrupt(); 


#endif /* CPU_INCLUDE_CICLO_CHECK_INTERRUPT_H_ */
