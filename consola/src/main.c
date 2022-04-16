#include <stdlib.h>
#include <stdio.h>
#include <commons/log.h>

int main(int argc, char* argv[]) {

    puts("Hello world!!");
    t_log* logger = log_create("./log.txt", "Consola", 1, LOG_LEVEL_ERROR);
    log_error(logger, "PRUEBA!");

    return 0;
}
