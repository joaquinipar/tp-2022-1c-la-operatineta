#include "../include/planificacion.h"

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
    pcb_t *nuevo_proceso1 = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso1);

    pcb_t *nuevo_proceso2 = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso2);

    pcb_t *nuevo_proceso3 = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso3);

    pcb_t *nuevo_proceso4 = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso4);

    pcb_t *nuevo_proceso5 = crear_proceso_prueba();
    ejecutar_proceso_nuevo(nuevo_proceso5);

    // le damos tiempo al plani de correr
    sleep(5); 

    finalizar_proceso(nuevo_proceso1);

    sleep(15);

    // La config tiene grado de multiprogramacion 2
    // A esta altura el primer proceso ya deberia estar terminado
    // deberia haber: 
    // 2 procesos en cola de listos
    // 1 procesos en cola de suspendidos listos
    // 1 proceso en cola de ejecucion
    // 1 proceso en cola de terminados
    CU_ASSERT_EQUAL(cantidad_procesos_listos(), 2);
    CU_ASSERT_EQUAL(cantidad_procesos_suspendidos_listos(), 2);
    CU_ASSERT_EQUAL(cantidad_procesos_ejecutando(), 1);
    CU_ASSERT_EQUAL(cantidad_procesos_terminados(), 1);

}