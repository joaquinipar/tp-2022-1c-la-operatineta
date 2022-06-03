#ifndef CPU_INCLUDE_SERVER_INTERRUPT_H_
#define CPU_INCLUDE_SERVER_INTERRUPT_H_

#include <signal.h>
#include <stdbool.h>
#include <utils/server.h>
#include <pthread.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/structs.h>
#include "configuracion_cpu.h"
#include "ciclo_instruccion.h"
#include <utils/logger.h>

pthread_t iniciar_server_interrupt();
int escuchar_conexiones_nuevas_interrupt(int server_interrupt_socket); 
bool procesar_conexion_interrupt(int cliente_socket);

#endif /* CPU_INCLUDE_SERVER_INTERRUPT_H_ */
