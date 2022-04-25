#include "../include/setup.h"

void setup_logger(){
    // Se hace una sola vez a nivel aplicacion
    logger_set_module("KERNEL");
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a KERNEL!!!");
}

