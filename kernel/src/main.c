#include "../include/main.h"

void inicializar_kernel(int argc, char* argv[]) {
    iniciar_config(argc,argv);
    inicializar_kernel_api();
    iniciar_monitor_colas_procesos();
}

int main(int argc, char* argv[]) {

    logger_set_module("KERNEL"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a KERNEL!!");

    inicializar_kernel(argc, argv);

    int *server_exit_code = NULL;

    pthread_t hilo_servidor = iniciar_server_kernel();
    pthread_join(hilo_servidor, (void *)server_exit_code);

    char *exit_msg = string_from_format("El servidor de Kernel finalizo con exit code: %p", server_exit_code);
    info_log("kernel/main.c@main", exit_msg);
    free(exit_msg);

    cerrar_kernel(); 
    return 0;
}
