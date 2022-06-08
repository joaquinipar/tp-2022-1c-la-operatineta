#ifndef INCLUDE_PLANIFICADOR_CORTO_PLAZO_H_
#define INCLUDE_PLANIFICADOR_CORTO_PLAZO_H_

#include "config_kernel.h"
#include <utils/logger.h>
#include "monitor_colas_procesos.h"
#include "proceso.h"
#include <commons/collections/list.h>
#include <commons/log.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void iniciar_planificador_corto_plazo();
void planificar_procesos();
void finalizar_planificador_corto_plazo();

#endif /* INCLUDE_PLANIFICADOR_CORTO_PLAZO_H_ */