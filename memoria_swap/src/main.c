#include "../include/main.h"

int main(int argc, char* argv[]) {
    logger_set_module("MEMORIA - SWAP"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a Memoria - Swamp!!");

    iniciar_config_mem_swap("./mem_swap.config");
    cerrar_mem_swap();

    return 0;
}
