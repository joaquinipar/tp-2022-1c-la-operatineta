#ifndef KERNEL_INCLUDE_STRUCT_H_
#define KERNEL_INCLUDE_STRUCT_H_

#include <stdlib.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <sys/time.h>
#include <utils/logger.h>
#include <utils/structs.h>

// Vamos a necesitar saber cuanto se tiene que bloquear asi que la lista de bloqueados va a tener esto
typedef struct bloqueo_proceso_t {
    int retardo;
    pcb_t *proceso;
} bloqueo_proceso_t;

#endif /* KERNEL_INCLUDE_STRUCT_H_ */
