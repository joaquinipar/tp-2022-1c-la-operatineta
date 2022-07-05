#include "../include/cerrar_mem_swap.h"


void cerrar_mem_swap() {

info_log("cerrar_mem_swap.c@cerrar_mem_swap", "Se comienza a cerrar aplicacion Memoria - Swap ");
destruir_array_mem(); 
destruir_estructura_mem_principal(); 
destruir_listas_globales_de_tablas();
destruir_list_swap(); 
destruir_estructura_mem_swap_config();
pthread_cancel(hilo_cpu); 
pthread_cancel(hilo_kernel);
pthread_mutex_destroy(&sem_procesar_conexion);
//cerrar_server_memoria(); 

}
