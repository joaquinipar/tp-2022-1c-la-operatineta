#ifndef INCLUDE_PLANIFICADOR_LARGO_PLAZO_H_
#define INCLUDE_PLANIFICADOR_LARGO_PLAZO_H_

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
#include <signal.h>

void iniciar_planificador_largo_plazo();
void controlar_grado_de_multiprogramacion();
void finalizar_planificador_largo_plazo();
bool grado_de_multiprogramacion_completo();

#endif /* INCLUDE_PLANIFICADOR_LARGO_PLAZO_H_ */