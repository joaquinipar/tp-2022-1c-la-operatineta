#include "main.h"

int main(int argc, char* argv[]) {

    

    // Se hace una sola vez a nivel aplicacion
    logger_set_module("CONSOLA");
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a CONSOLA!!!");

    debug_log("main.c", "DEBUG LOG!");
    info_log("main.c", "INFO LOG!");
    error_log("main.c", "PRUEBA!");

    hello_world();

    iniciar_config_consola("./ejemplo.config");
    print_config();
    info_log("main.c", "Se comienza a cerrar aplicacion CONSOLA ");
    destruir_estructura_consola_config();

    return 0;
}
