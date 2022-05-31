#include "../include/planificacion.h"

/* int suma(int a, int b);

int suma (int a, int b ) {
    return (a + b);
} */

pcb_t *crear_proceso_prueba() {
    pcb_t *nuevo_proceso = malloc(sizeof(pcb_t));
    nuevo_proceso->pid = obtener_siguiente_pid();
    nuevo_proceso->estado = ESTADO_PROCESO_NEW;
    nuevo_proceso->program_counter = 0;
    nuevo_proceso->tamanio = 160;

    return nuevo_proceso;
}

void test_planificacion_inicial() {

    // SETUP del test
    logger_set_module("KERNEL"); 
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a KERNEL!!");

    char* argumentos[] = {"tests.c", "./arch_config/kernel.config"};
    inicializar_kernel(2, argumentos);

    // PONER UN PROCESO EN COLA DE LISTOS
    pcb_t *nuevo_proceso = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso);

    nuevo_proceso = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso);

    nuevo_proceso = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso);

    // le damos tiempo al plani de correr
    sleep(5); 

    // A esta altura el primer proceso ya deberia estar ejecutando
    // otros 2 procesos en listos
    // nos fijamos que esten en la cola correcta
    CU_ASSERT_EQUAL(cantidad_procesos_listos(), 2);
    CU_ASSERT_EQUAL(cantidad_procesos_ejecutando(), 1);

    //CU_ASSERT_EQUAL(suma(0,2), 2);
}