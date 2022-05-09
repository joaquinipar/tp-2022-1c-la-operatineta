//
// Botonera v2 RELOADED. Est. 2021
//

#ifndef MEMORIA_SWAP_BOTONERA_H
#define MEMORIA_SWAP_BOTONERA_H

#include <CUnit/CUnit.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/logger.h>
#include <utils/server.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../../include/server_mem.h"
//#include "../../../kernel/include/config_kernel.h"
#include "../../../include/configuracion_mem_swap.h"
#include "botonera_helper.h"

int iniciar_cliente_test(int argc, char* ruta);

void probar_VALUE_TAB_PAG();
void probar_VALUE_TAB_PAG_con_varios_procesos();

#endif //MEMORIA_SWAP_BOTONERA_H
