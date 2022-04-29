#include "../include/cerrar_mem_swap.h"

void cerrar_mem_swap() {

info_log("cerrar_mem_swap.c@cerrar_mem_swap", "Se comienza a cerrar aplicacion Memoria - Swap ");
destruir_estructura_mem_swap_config();
destruir_estructura_mem_principal(); 

}