#ifndef KERNEL_INCLUDE_KERNEL_API_H_
#define KERNEL_INCLUDE_KERNEL_API_H_

#include "proceso.h"
#include <commons/string.h>
#include <stdbool.h>
#include <stdint.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include "config_kernel.h"
#include "struct.h"
#include "proceso.h"
#include <pthread.h>

void inicializar_kernel_api();
bool ejecutar_proceso_nuevo(pcb_t *proceso);
pid_t obtener_siguiente_pid ();

#endif /* KERNEL_INCLUDE_KERNEL_API_H_ */
