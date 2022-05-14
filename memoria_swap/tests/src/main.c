#include "../include/main.h"

/* correr desde /test/bin */
int main(void)
{
    CU_initialize_registry();

    pthread_mutex_init(&sum_mutex, NULL);

    CU_pSuite test_suite_config = CU_add_suite("Pruebas Config Memoria",NULL,NULL);
    CU_pSuite test_suite_mensajes = CU_add_suite("Pruebas Mensajes Memoria",NULL,NULL);
    CU_pSuite test_suite_algoritmos = CU_add_suite("Pruebas Algoritmos Memoria",NULL,NULL);
    //CU_basic_set_mode();

    // Agregar los tests aca abajo

    // Tests config
    CU_add_test(test_suite_config, "Test Iniciar Config", test_iniciar_config); // test_iniciar_config es lo que se importa y tiene los tests

    // Test mensajes E2E (con server)
    CU_add_test(test_suite_mensajes, "Test E2E mensaje VALUE_TAB_PAG", probar_VALUE_TAB_PAG);
    CU_add_test(test_suite_mensajes, "Test E2E mensaje VALUE_TAB_PAG con varios procesos", probar_VALUE_TAB_PAG_con_varios_procesos);

    // Test algoritmos reemplazo
    CU_add_test(test_suite_algoritmos, "Test Agregar proceso a lista de punteros clock", test_agregar_puntero_nuevo_clock);
    CU_add_test(test_suite_algoritmos, "Test Algoritmo de reemplazo", test_mover_puntero);

    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}