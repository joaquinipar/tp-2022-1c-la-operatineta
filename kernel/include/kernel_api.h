#ifndef KERNEL_INCLUDE_KERNEL_API_H_
#define KERNEL_INCLUDE_KERNEL_API_H_

#include <commons/string.h>
#include <stdbool.h>
#include <stdint.h>
#include <utils/server.h>
#include <utils/protocolo_comunicacion.h>
#include "config_kernel.h"
#include "structs.h"
#include <pthread.h>
#include "monitor_colas_procesos.h"
#include "planificador_corto_plazo.h"
#include "planificador_mediano_plazo.h"
#include <conexion_client.h>
#include "dispositivo_io.h"
#include "proceso.h"

void inicializar_kernel_api();
bool ejecutar_proceso_nuevo(pcb_t *proceso);
pid_t obtener_siguiente_pid ();
bool finalizar_proceso(pcb_t *proceso_actualizado);
bool bloquear_proceso(pcb_t *proceso_actualizado, int tiempo_bloqueo);
bool desalojar_proceso_interrupt(pcb_t *proceso_actualizado);


#endif /* KERNEL_INCLUDE_KERNEL_API_H_ */
