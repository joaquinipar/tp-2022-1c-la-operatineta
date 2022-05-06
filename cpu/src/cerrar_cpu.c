#include "../include/cerrar_cpu.h"

void cerrar_cpu() {
info_log("cerrar_cpu.c@cerrar_mem_swap", "Se comienza a cerrar aplicacion CPU ");
destruir_estructura_cpu_config();
free(datos_traduccion); 
//shutdown(socket_server_dispatch, SHUT_RD);
//shutdown(socket_server_interrupt, SHUT_RD);
}