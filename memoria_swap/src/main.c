#include "../include/main.h"

/*
    Para ejecutar la memoria-swap, el binario deberá recibir un argumento
        * Ruta al archivo de config
    ej:
    $ cd bin
    $ ./memoria_swap.out ../config/ejemplo.config
*/
int main(int argc, char* argv[]) {
    logger_set_module("MEMORIA - SWAP"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a Memoria - Swamp!!");

    iniciar_config(argc,argv);
    iniciar_server_mem_swap();
    cerrar_mem_swap();

    return 0;
}
