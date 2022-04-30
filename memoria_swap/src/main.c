#include "../include/main.h"

int main(int argc, char* argv[]) {
    logger_set_module("MEMORIA - SWAP"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a Memoria - Swamp!!");

    iniciar_config(argc,argv);

    info_log("main.c","Se inicializa estructuras de Memoria"); 
    iniciar_memoria_principal();
    iniciar_listas_globales_de_tablas();
    iniciar_swap();






    iniciar_server_mem_swap();
    cerrar_mem_swap();
    return 0;
}
