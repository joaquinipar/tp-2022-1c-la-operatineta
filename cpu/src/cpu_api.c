#include "../include/cpu_api.h"


void iniciar_modulo(pcb_t* proceso_recibido){

format_info_log("cpu_api.c@iniciar_modulo", "Comienza ciclo de instruccion - PID: %d", proceso_recibido->pid); 

 iniciar_ciclo_instruccion(proceso_recibido);


}

