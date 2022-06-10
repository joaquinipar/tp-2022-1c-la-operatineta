#include "../include/inicializacion.h"

void inicializar_kernel(int argc, char* argv[]) {

    info_log("config_kernel.h@iniciar_config", "Inicializando kernel");

    iniciar_config(argc,argv);
    inicializar_kernel_api();
    iniciar_monitor_colas_procesos();
    iniciar_planificador_largo_plazo();
    iniciar_planificador_corto_plazo();
    iniciar_planificador_mediano_plazo();
    iniciar_conexion_cpu_dispatch();
    iniciar_conexion_cpu_interrupt();
    iniciar_dispositivo_io();
}