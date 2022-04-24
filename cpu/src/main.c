#include "../include/main.h"

/*Para ejecutar 
        OP1 -> valgrind ./bin/cpu.out ./arch_config/cpu.config (Parado en la carpeta CPU)
        OP2->  make start (Se incluyo en la config de las settings el argumento - Parado en la carpeta CPU)
        */



int main(int argc, char* argv[]) {
    logger_set_module("CPU"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a CPU!!");

    iniciar_config(argc,argv);


    cerrar_cpu();
    return 0;
}
