#include "../include/configuracion_mem_swap.tests.h"

void test_iniciar_config () {
    char* test_argv[2] = {"sssss", "../arch_config/memoria.config"};
    iniciar_config(2, test_argv);
    CU_ASSERT_EQUAL(mem_swap_config->tam_memoria, 4096);
    CU_ASSERT_STRING_EQUAL(mem_swap_config->ip_escucha, "127.0.0.1");
}