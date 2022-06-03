#include <stdio.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include "../include/planificacion.h"

int main(void)
{
    CU_initialize_registry();
    CU_pSuite suite_prueba=CU_add_suite("Tests de Kernel", NULL, NULL);
    //CU_basic_set_mode();

    //CU_add_test(suite_prueba,"uno", test_suma);
    CU_add_test(suite_prueba, "Prueba Ciclo Inicial", test_planificacion_inicial);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}