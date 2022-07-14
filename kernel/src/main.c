#include "../include/main.h"

void sighandler(int signal){
  cerrar_server_kernel(signal);
  finalizar_planificador_largo_plazo();
  finalizar_planificador_mediano_plazo();
  finalizar_planificador_corto_plazo();
  finalizar_dispositivo_io();
  destruir_monitor_colas_procesos();
}

int main(int argc, char* argv[]) {

    logger_set_module("KERNEL"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a KERNEL!!");
    signal(SIGINT, sighandler);
    inicializar_kernel(argc, argv);
    /*&& !iniciar_conexion_cpu_interrupt() )faltaria sumar la conexion de memoria */

    int *server_exit_code = NULL;

    iniciar_conexion_memoria(); 

    pthread_t hilo_servidor = iniciar_server_kernel();
    pthread_join(hilo_servidor, (void *)server_exit_code);

    if(!iniciar_conexion_cpu_dispatch()) {
      cerrar_kernel();
      return EXIT_FAILURE;
    }
 
    char *exit_msg = string_from_format("El servidor de Kernel finalizo con exit code: %p", server_exit_code);
    info_log("kernel/main.c@main", exit_msg);
    free(exit_msg);

    cerrar_kernel(); 
    return 0;
}
