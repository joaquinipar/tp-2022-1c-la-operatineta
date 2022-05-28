#include "../include/main.h"

int main(int argc, char* argv[]) {

    logger_set_module("KERNEL"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a KERNEL!!");

    inicializar_kernel(argc, argv);
    /*&& !iniciar_conexion_cpu_interrupt() )faltaria sumar la conexion de memoria */

    int *server_exit_code = NULL;

    pthread_t hilo_servidor = iniciar_server_kernel();
    pthread_join(hilo_servidor, (void *)server_exit_code);

    if(!iniciar_conexion_cpu_dispatch()) {
      cerrar_kernel();
      return EXIT_FAILURE;
    }

   
    pcb_t* un_proceso = malloc(sizeof(pcb_t)); 
    un_proceso->pid = 846; 
    un_proceso->tamanio= 40; 
    un_proceso->program_counter= 6;
    un_proceso->tabla_paginas= 8;
    un_proceso->estado= 1;
    un_proceso->estimacion_rafaga= 85;
    un_proceso->estimacion= 74;
    un_proceso->duracion_ultima_rafaga= 32;
    //(un_proceso->rafaga_actual).inicio = 1.71;
    // (un_proceso->rafaga_actual).fin = 3.63; 
    t_list* instrucciones = list_create(); 
    instruccion_t* una = malloc(sizeof(instruccion_t)); 
    una->instruccion = 0; 
    una->argumentos[0] = 8; 
    una->argumentos[1]= 3; 
    list_add(instrucciones, una); 
    instruccion_t* dos = malloc(sizeof(instruccion_t)); 
    dos->instruccion = 2; 
    dos->argumentos[0] = 2015; 
    dos->argumentos[1]= 9; 
    list_add(instrucciones, dos); 
    un_proceso->lista_instrucciones = instrucciones;

    // send_prueba(socket_cliente_cpu_dispatch);
   
    enviar_mensaje_ejecutar(un_proceso); 
 
    char *exit_msg = string_from_format("El servidor de Kernel finalizo con exit code: %p", server_exit_code);
    info_log("kernel/main.c@main", exit_msg);
    free(exit_msg);

    cerrar_kernel(); 
    return 0;
}
