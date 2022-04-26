#include "../include/configuracion_cpu.tests.h"

void test_iniciar_config () {
    char* test_argv[2] = {"sssss", "../arch_config/cpu.config"};
    iniciar_config(2, test_argv);
    CU_ASSERT_EQUAL(cpu_config->puerto_escucha_dispatch, 1234);
    CU_ASSERT_STRING_EQUAL(cpu_config->ip_escucha, "127.0.0.1");
}