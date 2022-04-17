#include "main.h"
#include <utils/hello.h>
#include <utils/logger.h>

int main(int argc, char* argv[]) {

    puts("Hello world!!");

    logger_set_module("CONSOLA");

    debug_log("main.c", "DEBUG LOG!");
    info_log("main.c", "INFO LOG!");
    error_log("main.c", "PRUEBA!");

    hello_world();

    iniciar_config_consola("../../config/ejemplo.config");
    print_config();

    destruir_estructura_consola_config();
    log_destroy(logger);

    return 0;
}
