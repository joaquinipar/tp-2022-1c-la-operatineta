#ifndef MEMORIA_SWAP_ALGORITMOS_TEST_H
#define MEMORIA_SWAP_ALGORITMOS_TEST_H

#include <CUnit/CUnit.h>
#include <utils/logger.h>
#include "../../../include/algoritmos_reemplazo/algoritmos_reemplazo.h"
#include "../../../include/memoria_principal.h"

/**
 * Test funcion de agregar nuevo proceso a la lista de punteros clock.
 */
void test_agregar_puntero_nuevo_clock();

void test_mover_puntero();
void test_clock();

#endif //MEMORIA_SWAP_ALGORITMOS_TEST_H
