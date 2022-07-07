#include "../include/setup.h"

void setup_logger(){
    // Se hace una sola vez a nivel aplicacion
    logger_set_module("CONSOLA");
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a CONSOLA!!!");
}

void cerrar_consola(){
    info_log("main.c", "Se comienza a cerrar aplicacion CONSOLA ");
    destruir_estructura_consola_config();
    info_log("main.c", "Se cierra aplicacion CONSOLA ");
    
}