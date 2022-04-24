#include "../include/main.h"


int main(int argc, char* argv[]) {
    logger_set_module("CPU"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a CPU!!");

    iniciar_config_cpu("../cpu.config");


    cerrar_cpu();
    return 0;
}
