#include "../../include/algoritmos_reemplazo/algoritmos_test.h"

void test_agregar_puntero_nuevo_clock(){

    char* test_argv[2] = {"sssss", "../arch_config/memoria.config"};
    iniciar_config(2, test_argv);
    iniciar_memoria_principal();
    iniciar_lista_punteros_clock_modificado_fija();


    int pid = 6655;
    agregar_puntero_nuevo_clock(pid);

    int proceso_en_lista_punteros(puntero_clock_modificado* puntero) {
        return puntero->pid == pid;
    }

    int pid_esta_en_lista_punteros = list_any_satisfy(punteros_procesos, (void*) &proceso_en_lista_punteros);

    CU_ASSERT_EQUAL(list_size(punteros_procesos), 1);
    CU_ASSERT_EQUAL(pid_esta_en_lista_punteros, 1);

    destruir_lista_de_punteros_clock();
    destruir_array_mem();

}

void test_mover_puntero(){

    /*
     *
     */


}


