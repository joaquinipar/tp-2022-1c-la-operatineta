#ifndef PROCESOS_H
#define PROCESOS_H

#include "config_kernel.h"
#include <utils/logger.h>
#include "structs.h"
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <matelib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

void inicializar_monitor_procesos();
pcb_t *crear_proceso(int id, int socket, int socket_memoria);
void proceso_destruir(pcb_t *);
void proceso_iniciar_rafaga(pcb_t *);
void proceso_finalizar_rafaga(pcb_t *);
void proceso_ejecutar(pcb_t *);
void proceso_iniciar_espera(pcb_t *);
void proceso_finalizar_espera(pcb_t *);

float timedifference_msec(struct timeval, struct timeval);

#endif
