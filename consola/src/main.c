#include "main.h"

int main(int argc, char* argv[]) {

    puts("Hello world!!");

    t_log* logger = log_create("./log.txt", "Consola", 1, LOG_LEVEL_INFO);
    
    log_debug(logger, "DEBUG LOG!");
    log_info(logger, "INFO LOG!");
    log_error(logger, "PRUEBA!");

    //hello_world(); perdon luis pero no anda mas el hello world :(

    iniciar_config_consola("../config/ejemplo.config");
    print_config();

    destruir_estructura_consola_config();
    log_destroy(logger);

    return 0;
}
