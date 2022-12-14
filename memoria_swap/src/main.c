#include "../include/main.h"

int main(int argc, char* argv[]) {
    logger_set_module("MEMORIA - SWAP"); 
    logger_set_loglevel(LOG_LEVEL_INFO);
    info_log("main.c", "Bienvenido al modulo MEMORIA!!");

    iniciar_config(argc,argv);

    info_log("main.c","Se inicializa estructuras administrativas de Memoria"); 
    iniciar_memoria(); 



    iniciar_server_mem_swap(mem_swap_config->ip_escucha, mem_swap_config->puerto_escucha);
    cerrar_mem_swap();
    return 0;
}
