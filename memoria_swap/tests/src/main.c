#include "../include/main.h"

/* correr desde /test/bin */
int main(void)
{
    CU_initialize_registry();
    CU_pSuite test_suite_name = CU_add_suite("Pruebas Memoria",NULL,NULL);
    //CU_basic_set_mode();

    // Agregar los tests aca abajo
    CU_add_test(test_suite_name, "Test Iniciar Config", test_iniciar_config); // test_iniciar_config es lo que se importa y tiene los tests

    CU_add_test(test_suite_name, "Test mensaje VALUE_TAB_PAG", probar_VALUE_TAB_PAG);

    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}