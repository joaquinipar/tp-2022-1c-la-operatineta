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

bool admitir_proceso_nuevo(int socket);

#endif /* KERNEL_INCLUDE_KERNEL_API_H_ */
