#include "../../include/algoritmos_reemplazo/algoritmos_test.h"

void test_agregar_puntero_nuevo_clock(){

    char* test_argv[2] = {"sssss", "../arch_config/memoria.config"};
    iniciar_config(2, test_argv);
    iniciar_memoria_principal();
    iniciar_lista_punteros_clock();


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

    char* test_argv[2] = {"sssss", "../arch_config/memoria.config"};
    iniciar_config(2, test_argv);
    iniciar_memoria_principal();

    // Setup array marcos, lo voy llenando

    int pid_base = 100;
    int count = 0;

    mem_ppal = malloc(sizeof(mem_principal_t));
    mem_ppal->cant_marcos = 64;
    mem_swap_config->marcos_por_proceso = 4;

    iniciar_lista_punteros_clock();


    printf("cant marcos %i cant marcos x proceso %i",mem_ppal->cant_marcos, mem_swap_config->marcos_por_proceso);
    puts("------------------\n");
    for(int i=0; i < mem_ppal->cant_marcos; i++){
        array_marcos[i].pid = pid_base;
        array_marcos[i].estado = 1;
        array_marcos[i].pagina = malloc(sizeof(pagina_2do_nivel_t));
        array_marcos[i].pagina->nro_pagina = 10000+i; // cualquier pagina....
        array_marcos[i].pagina->marco = i; // cualquier marco.... no interesa
        array_marcos[i].pagina->bit_uso = 1;
        array_marcos[i].pagina->bit_modificado = 0;
        array_marcos[i].pagina->bit_presencia = 1; // asumo que estan todos en mem por ahora...

        printf("marco %d pid %d\n", i, pid_base);

        printf("Marco: %i Pid: %i Estado: %i", i, pid_base, 1);

        count++;

        if(count == mem_swap_config->marcos_por_proceso){

            agregar_puntero_nuevo_clock(pid_base);
            setear_marco_a_puntero_clock(pid_base, i);

            count = 0;
            pid_base++;
        }

    }
    // end setup
    puts("------------------\n");




    puts("holaa stop it");

    /*
        marcos          pid
    0	1	2	3		100
    4	5	6	7		101
    8	9	10	11		102
    12	13	14	15		103

     Puntero de pid 103: Apunta a 15.

     Test: Corre algoritmo de reemplazo -> Escoge al marco 15 como victima, y mueve el puntero a 12.

     */
    uint32_t victim103 = correr_algoritmo_reemplazo(103);
    uint32_t victim100 = correr_algoritmo_reemplazo(100);

    format_warning_log("algoritmos_test.c@test_mover_puntero", "victima 103: %i", victim103);
    format_warning_log("algoritmos_test.c@test_mover_puntero", "nuevo puntero 103: %i", obtener_puntero_clock(103));
    format_warning_log("algoritmos_test.c@test_mover_puntero", "victima 100: %i", victim100);
    format_warning_log("algoritmos_test.c@test_mover_puntero", "nuevo puntero 100: %i", obtener_puntero_clock(100));

    //////// Proceso 103
    // marco victima del proceso 103 debería ser 10015
    CU_ASSERT_EQUAL(15, victim103);
    // El nuevo puntero al marco del proceso 103 debería ser 10012
    CU_ASSERT_EQUAL(12,  obtener_puntero_clock(103));
    ////////

    //////// Proceso 100
    // El marco victima del proceso 100 debería ser 3
    CU_ASSERT_EQUAL(3, victim100);
    // El nuevo puntero al marco del proceso 100 debería ser 10012
    CU_ASSERT_EQUAL(0, obtener_puntero_clock(100));
    ////////
}

void test_clock(){

    char* test_argv[2] = {"sssss", "../arch_config/memoria_test_clk.config"};
    iniciar_config(2, test_argv);
    iniciar_memoria_principal();

    // Setup array marcos, lo voy llenando

    int pid_base = 100;
    int count = 0;

    mem_ppal = malloc(sizeof(mem_principal_t));
    mem_ppal->cant_marcos = 64;
    mem_swap_config->marcos_por_proceso = 4;

    iniciar_lista_punteros_clock();


    printf("cant marcos %i cant marcos x proceso %i",mem_ppal->cant_marcos, mem_swap_config->marcos_por_proceso);
    puts("------------------\n");
    for(int i=0; i < mem_ppal->cant_marcos; i++){
        array_marcos[i].pid = pid_base;
        array_marcos[i].estado = 1;
        array_marcos[i].pagina = malloc(sizeof(pagina_2do_nivel_t));
        array_marcos[i].pagina->nro_pagina = 10000+i; // cualquier pagina....
        array_marcos[i].pagina->marco = i; // cualquier marco.... no interesa
        array_marcos[i].pagina->bit_uso = 1;
        array_marcos[i].pagina->bit_modificado = 0;
        array_marcos[i].pagina->bit_presencia = 1; // asumo que estan todos en mem por ahora...

        printf("marco %d pid %d\n", i, pid_base);

        printf("Marco: %i Pid: %i Estado: %i", i, pid_base, 1);

        count++;

        if(count == mem_swap_config->marcos_por_proceso){

            agregar_puntero_nuevo_clock(pid_base);
            setear_marco_a_puntero_clock(pid_base, i);

            count = 0;
            pid_base++;
        }

    }
    // end setup
    puts("------------------\n");




    puts("holaa stop it");

    /*
        marcos          pid
    0	1	2	3		100
    4	5	6	7		101
    8	9	10	11		102
    12	13	14	15		103

     Puntero de pid 103: Apunta a 15.

     Test: Corre algoritmo de reemplazo -> Escoge al marco 15 como victima, y mueve el puntero a 12.

     */
    uint32_t victim103 = correr_algoritmo_reemplazo(103);
    uint32_t victim100 = correr_algoritmo_reemplazo(100);

    format_warning_log("algoritmos_test.c@test_clock", "victima 103: %i", victim103);
    format_warning_log("algoritmos_test.c@test_clock", "nuevo puntero 103: %i", obtener_puntero_clock(103));
    format_warning_log("algoritmos_test.c@test_clock", "victima 100: %i", victim100);
    format_warning_log("algoritmos_test.c@test_clock", "nuevo puntero 100: %i", obtener_puntero_clock(100));

    //////// Proceso 103
    // La pagina del marco victima del proceso 103 debería ser 10015
    CU_ASSERT_EQUAL(15, victim103);
    // El nuevo puntero al marco del proceso 103 debería ser 10012
    CU_ASSERT_EQUAL(12,  obtener_puntero_clock(103));
    ////////

    //////// Proceso 100
    // El marco victima del proceso 100 debería ser 3
    CU_ASSERT_EQUAL(3, victim100);
    // El nuevo puntero al marco del proceso 100 debería ser 10012
    CU_ASSERT_EQUAL(0, obtener_puntero_clock(100));
    ////////
}

