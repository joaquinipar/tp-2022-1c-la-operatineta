#ifndef INICIALIZACION_KERNEL_H
#define INICIALIZACION_KERNEL_H

    #include "config_kernel.h"
    #include "kernel_api.h"
    #include "monitor_colas_procesos.h"
    #include "planificador_largo_plazo.h"
    #include "planificador_corto_plazo.h"
    #include "dispatcher.h"
    #include "dispositivo_io.h"

    void inicializar_kernel(int argc, char* argv[]);

#endif