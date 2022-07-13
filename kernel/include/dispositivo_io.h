#ifndef DISPOSITIVO_IO_H
#define DISPOSITIVO_IO_H

#include "structs.h"
#include "monitor_colas_procesos.h"
#include <semaphore.h>
#include <pthread.h>
#include <utils/logger.h>
#include "planificador_mediano_plazo.h"

void iniciar_dispositivo_io();
void destruir_dispositivo_io();
void incrementar_cantidad_procesos_bloqueados();
void finalizar_dispositivo_io();

#endif
