#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>
#include <utils/hello.h>

int main(int argc, char* argv[]) {

    puts("Hello world!!");

    t_log* logger = log_create("./log.txt", "Consola", 1, LOG_LEVEL_INFO);
    
    log_debug(logger, "DEBUG LOG!");
    log_info(logger, "INFO LOG!");
    log_error(logger, "PRUEBA!");

    hello_world();

    return 0;
}
