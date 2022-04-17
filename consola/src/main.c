#include "main.h"
#include <utils/logger.h>

int main(int argc, char* argv[]) {

    puts("Hello world!!");

    logger_set_module("CONSOLA");

    t_log* logger = log_create("./log.txt", "Consola", 1, LOG_LEVEL_INFO);
    
    debug_log("main.c", "DEBUG LOG!");
    info_log("main.c", "INFO LOG!");
    error_log("main.c", "PRUEBA!");

    //hello_world(); perdon luis pero no anda mas el hello world :(

    iniciar_config_consola("../../config/ejemplo.config");
    print_config();

    destruir_estructura_consola_config();
    log_destroy(logger);

    return 0;
}
