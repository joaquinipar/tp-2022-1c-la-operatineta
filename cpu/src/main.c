#include "../include/main.h"

/*Para ejecutar 
        OP1 -> valgrind ./bin/cpu.out ./arch_config/cpu.config (Parado en la carpeta CPU)
        OP2->  make start (Se incluyo en la config de las settings el argumento - Parado en la carpeta CPU)
        */

int main(int argc, char* argv[]) {
    logger_set_module("CPU"); 
    logger_set_loglevel(LOG_LEVEL_INFO);
    info_log("main.c", "Bienvenido a CPU!!");

    iniciar_config(argc,argv);
    if(!iniciar_conexion_memoria()){
      cerrar_cpu();
      return EXIT_FAILURE;
    }
    iniciar_tlb(); 
    enviar_mensaje_inicial_configuracion(); 
   // send_prueba(socket_cliente_cpu); 
    int *server_exit_code = NULL;
    pthread_t hilo_dispatch = iniciar_server_dispatch();
    pthread_t hilo_interrupt = iniciar_server_interrupt();
    
    pthread_join(hilo_dispatch, (void *)server_exit_code);
    char *exit_msg = string_from_format("El servidor de Dispatch finalizo con exit code: %p", server_exit_code);
    info_log("cpu/main.c@main", exit_msg);
    free(exit_msg);

    int *server_exit_code_inter = NULL;
    pthread_join(hilo_interrupt, (void *)server_exit_code_inter);
  
    char *exit_msg_interrupt = string_from_format("El servidor de Interrupt finalizo con exit code: %p", server_exit_code_inter);
    info_log("cpu/main.c@main", exit_msg_interrupt);
    free(exit_msg_interrupt);

    cerrar_cpu();
    return 0;
}
