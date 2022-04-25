#include "../include/main.h"

int main(void)
{
    CU_initialize_registry();
    CU_pSuite test_suite_name = CU_add_suite("Pruebas Memoria",NULL,NULL);
    //CU_basic_set_mode();

    CU_add_test(test_suite_name, "Test Iniciar Config", test_iniciar_config);

    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}