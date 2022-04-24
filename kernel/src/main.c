#include "../include/main.h"


int main(int argc, char* argv[]) {
    logger_set_module("KERNEL"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a KERNEL!!");

    iniciar_config(argc,argv);

    cerrar_kernel(); 

    return 0;
}
