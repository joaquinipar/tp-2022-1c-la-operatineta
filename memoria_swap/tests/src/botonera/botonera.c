#include "../../include/botonera/botonera.h"


/*
 * Para usar la botonera, se deberá tener corriendo el servidor correspondiente.
 * La idea es hacer el cliente de cpu, kernel... los que hablan con memoria basicamente, y aparte tenemos
 * el proceso de memoria andando, y vemos como va respondiendo.
 */
int iniciar_cliente_test(int argc, char* ruta){

    // int crear_conexion(char *ip, char *puerto, char *nameServer, char *ubicacion) {

    // Inicio config mem
    char* test_argv[2] = {"sssss", ruta }; // "../arch_config/memoria.config"
    // Por que levanto el archivo de memoria? Tengo que saber la IP y el puerto donde esta corriendo.
    // El archivo de config debe coincidir con el que tenemos corriendo en el momento
    iniciar_config(argc, test_argv); // argc = 2



    // int crear_conexion(char *ip, char *puerto, char *nameServer, char *ubicacion)
    int sockett = crear_conexion(mem_swap_config->ip_escucha, mem_swap_config->puerto_escucha, "botonera", "tests/botonera.c@iniciar_cliente_test");

    return sockett; // shush!
}

void probar_VALUE_TAB_PAG() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria
    pthread_t hilo =crear_server_botonera_memoria();
    sleep(1);
    uint32_t pid = 12344;
    uint32_t tamanio = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";


    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "Pid pasado x parametro: ", pid);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "tamanio pasado x parametro: ", tamanio);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%s", "config pasado x parametro: ", ruta_config);


    int sockett = iniciar_cliente_test(2, ruta_config);

    sleep(1);

    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid, tamanio);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel;
    recv(sockett, &valor_tabla_1er_nivel, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    sleep(1);

    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    cerrar_server_botonera_memoria(sockett);

    desconectar_cliente(sockett);

    pthread_cancel(hilo);

    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);
    sleep(1);
    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel, 0);
    warning_log("botonera.c@probar_VALUE_TAB_PAG","------------------------------------------");
}


void probar_VALUE_TAB_PAG_con_varios_procesos() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG_con_varios_procesos","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria
    pthread_t hilo = crear_server_botonera_memoria();
    sleep(1);

    uint32_t pid1 = 123;
    uint32_t tamanio1 = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";

    uint32_t  pid2 = 255;
    uint32_t tamanio2 = 512;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 2048; // este va a fallar xq se excede

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 4096; // este va a fallar xq se excede

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%s", "config pasado x parametro: ", ruta_config);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "Pid pasado x parametro: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "tamanio pasado x parametro: ", tamanio1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "Pid pasado x parametro: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "tamanio pasado x parametro: ", tamanio2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "Pid pasado x parametro: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "tamanio pasado x parametro: ", tamanio3);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "Pid pasado x parametro: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "tamanio pasado x parametro: ", tamanio4);

    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid4, tamanio4);

    sleep(1);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid1;
    recv(sockett, &valor_tabla_1er_nivel_pid1, sizeof(uint32_t), false);

    uint32_t codop2;
    recv(sockett, &codop2, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid2;
    recv(sockett, &valor_tabla_1er_nivel_pid2, sizeof(uint32_t), false);

    uint32_t codop3;
    recv(sockett, &codop3, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid3;
    recv(sockett, &valor_tabla_1er_nivel_pid3, sizeof(uint32_t), false);

    uint32_t codop4;
    recv(sockett, &codop4, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid4;
    recv(sockett, &valor_tabla_1er_nivel_pid4, sizeof(uint32_t), false);



    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    info_log("botonera.c@probar_VALUE_TAB_PAG","Este caso debería fallar porque le esta mandando un size de proceso muy grande");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", -1);


    info_log("botonera.c@probar_VALUE_TAB_PAG","Este caso debería fallar porque le esta mandando un size de proceso muy grande");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", -1);




    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    cerrar_server_botonera_memoria(sockett);

    desconectar_cliente(sockett);
    pthread_cancel(hilo);
    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);


    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, -1);
    CU_ASSERT_EQUAL(codop4, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, -1);

    warning_log("botonera.c@probar_VALUE_TAB_PAG_con_varios_procesos","------------------------------------------");
}


void probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria
    pthread_t hilo = crear_server_botonera_memoria();
    sleep(1);

    uint32_t pid1 = 123;
    uint32_t tamanio1 = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";

    uint32_t  pid2 = 255;
    uint32_t tamanio2 = 1024;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 1024;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 1024;

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%s", "config pasado x parametro: ", ruta_config);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio3);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio4);

    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid4, tamanio4);

    sleep(1);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid1;
    recv(sockett, &valor_tabla_1er_nivel_pid1, sizeof(uint32_t), false);

    uint32_t codop2;
    recv(sockett, &codop2, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid2;
    recv(sockett, &valor_tabla_1er_nivel_pid2, sizeof(uint32_t), false);

    uint32_t codop3;
    recv(sockett, &codop3, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid3;
    recv(sockett, &valor_tabla_1er_nivel_pid3, sizeof(uint32_t), false);

    uint32_t codop4;
    recv(sockett, &codop4, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid4;
    recv(sockett, &valor_tabla_1er_nivel_pid4, sizeof(uint32_t), false);



    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);


    format_warning_log("test", "valor_tabla_1er_nivel_pid1: %i", valor_tabla_1er_nivel_pid1);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid1, valor_tabla_1er_nivel_pid1, 0);



    uint32_t codopRTA1;
    recv(sockett, &codopRTA1, sizeof(uint32_t), false);

    uint32_t PIDRTA1;
    recv(sockett, &PIDRTA1, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID1;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA1 RECIBIDO: ", codopRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA1 RECIBIDO: ", PIDRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA1 Esperado: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 Esperado: ", 0);



    format_warning_log("test", "valor_tabla_1er_nivel_pid2: %i", valor_tabla_1er_nivel_pid2);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid2, valor_tabla_1er_nivel_pid2, 0);

    uint32_t codopRTA2;
    recv(sockett, &codopRTA2, sizeof(uint32_t), false);

    uint32_t PIDRTA2;
    recv(sockett, &PIDRTA2, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID2;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID2, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA2 RECIBIDO: ", codopRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA2 RECIBIDO: ", PIDRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA2 Esperado: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 Esperado: ", 4);





    format_warning_log("test", "valor_tabla_1er_nivel_pid3: %i", valor_tabla_1er_nivel_pid3);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid3, valor_tabla_1er_nivel_pid3, 0);

    uint32_t codopRTA3;
    recv(sockett, &codopRTA3, sizeof(uint32_t), false);

    uint32_t PIDRTA3;
    recv(sockett, &PIDRTA3, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID3;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID3, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA3 RECIBIDO: ", codopRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA3 RECIBIDO: ", PIDRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA3 Esperado: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 Esperado: ", 8);


    format_warning_log("test", "valor_tabla_1er_nivel_pid4: %i", valor_tabla_1er_nivel_pid4);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid4, valor_tabla_1er_nivel_pid4, 0);

    uint32_t codopRTA4;
    recv(sockett, &codopRTA4, sizeof(uint32_t), false);

    uint32_t PIDRTA4;
    recv(sockett, &PIDRTA4, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID4;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID4, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA4 RECIBIDO: ", codopRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA4 RECIBIDO: ", PIDRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "PIDRTA4 Esperado: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 Esperado: ", 12);






    // POSICION_TABLA_2DO_NIVEL_PID1



    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    cerrar_server_botonera_memoria(sockett);

    desconectar_cliente(sockett);
    pthread_cancel(hilo);
    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);


    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, 2);
    CU_ASSERT_EQUAL(codop4, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, 3);

    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID1, 0);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID2, 4);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID3, 8);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID4, 12);

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos","------------------------------------------");
}



void probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria
    pthread_t hilo = crear_server_botonera_memoria();
    sleep(1);

    uint32_t pid1 = 123;
    uint32_t tamanio1 = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";

    uint32_t  pid2 = 255;
    uint32_t tamanio2 = 1024;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 1024;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 1024;

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%s", "config pasado x parametro: ", ruta_config);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio3);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "Pid pasado x parametro: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "tamanio pasado x parametro: ", tamanio4);

    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid4, tamanio4);

    sleep(1);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid1;
    recv(sockett, &valor_tabla_1er_nivel_pid1, sizeof(uint32_t), false);

    uint32_t codop2;
    recv(sockett, &codop2, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid2;
    recv(sockett, &valor_tabla_1er_nivel_pid2, sizeof(uint32_t), false);

    uint32_t codop3;
    recv(sockett, &codop3, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid3;
    recv(sockett, &valor_tabla_1er_nivel_pid3, sizeof(uint32_t), false);

    uint32_t codop4;
    recv(sockett, &codop4, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid4;
    recv(sockett, &valor_tabla_1er_nivel_pid4, sizeof(uint32_t), false);



    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);


    format_warning_log("test", "valor_tabla_1er_nivel_pid1: %i", valor_tabla_1er_nivel_pid1);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid1, valor_tabla_1er_nivel_pid1, 0);



    uint32_t codopRTA1;
    recv(sockett, &codopRTA1, sizeof(uint32_t), false);

    uint32_t PIDRTA1;
    recv(sockett, &PIDRTA1, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID1;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA1 RECIBIDO: ", codopRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA1 RECIBIDO: ", PIDRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA1 Esperado: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 Esperado: ", 0);



    format_warning_log("test", "valor_tabla_1er_nivel_pid2: %i", valor_tabla_1er_nivel_pid2);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid2, valor_tabla_1er_nivel_pid2, 0);

    uint32_t codopRTA2;
    recv(sockett, &codopRTA2, sizeof(uint32_t), false);

    uint32_t PIDRTA2;
    recv(sockett, &PIDRTA2, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID2;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID2, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA2 RECIBIDO: ", codopRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA2 RECIBIDO: ", PIDRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA2 Esperado: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 Esperado: ", 4);





    format_warning_log("test", "valor_tabla_1er_nivel_pid3: %i", valor_tabla_1er_nivel_pid3);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid3, valor_tabla_1er_nivel_pid3, 0);

    uint32_t codopRTA3;
    recv(sockett, &codopRTA3, sizeof(uint32_t), false);

    uint32_t PIDRTA3;
    recv(sockett, &PIDRTA3, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID3;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID3, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA3 RECIBIDO: ", codopRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA3 RECIBIDO: ", PIDRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA3 Esperado: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 Esperado: ", 8);


    format_warning_log("test", "valor_tabla_1er_nivel_pid4: %i", valor_tabla_1er_nivel_pid4);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid4, valor_tabla_1er_nivel_pid4, 0);

    uint32_t codopRTA4;
    recv(sockett, &codopRTA4, sizeof(uint32_t), false);

    uint32_t PIDRTA4;
    recv(sockett, &PIDRTA4, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID4;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID4, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA4 RECIBIDO: ", codopRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA4 RECIBIDO: ", PIDRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "PIDRTA4 Esperado: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 Esperado: ", 12);


    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos","mando 4 msjs de opcode acceso 2do nivel");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid1, POSICION_TABLA_2DO_NIVEL_PID1, 0);

    uint32_t opcodee1;
    recv(sockett, &opcodee1, sizeof(uint32_t), false);
    uint32_t pidd1;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco1;
    recv(sockett, &marco1, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA1 RECIBIDO: ", opcodee1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd1 RECIBIDO: ", pidd1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd1 Esperado: ", 123);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco RECIBIDO: ", marco1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco Esperado: ", 0);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid2, POSICION_TABLA_2DO_NIVEL_PID2, 0);

    uint32_t opcodee2;
    recv(sockett, &opcodee2, sizeof(uint32_t), false);
    uint32_t pidd2;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco2;
    recv(sockett, &marco2, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA2 RECIBIDO: ", opcodee2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd2 RECIBIDO: ", pidd2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd2 Esperado: ", 255);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco RECIBIDO: ", marco2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco Esperado: ", 1);
    warning_log("----","------------------------------------------------");



    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid3, POSICION_TABLA_2DO_NIVEL_PID3, 0);

    uint32_t opcodee3;
    recv(sockett, &opcodee3, sizeof(uint32_t), false);
    uint32_t pidd3;
    recv(sockett, &pidd3, sizeof(uint32_t), false);
    uint32_t marco3;
    recv(sockett, &marco3, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA3 RECIBIDO: ", opcodee3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd3 RECIBIDO: ", pidd3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd3 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco RECIBIDO: ", marco3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco Esperado: ", 2);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid4, POSICION_TABLA_2DO_NIVEL_PID4, 0);

    uint32_t opcodee4;
    recv(sockett, &opcodee4, sizeof(uint32_t), false);
    uint32_t pidd4;
    recv(sockett, &pidd4, sizeof(uint32_t), false);
    uint32_t marco4;
    recv(sockett, &marco4, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP RTA4 RECIBIDO: ", opcodee4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd4 RECIBIDO: ", pidd4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "pidd4 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco RECIBIDO: ", marco4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos", "%s%d", "marco Esperado: ", 3);
    warning_log("----","------------------------------------------------");




    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    cerrar_server_botonera_memoria(sockett);

    desconectar_cliente(sockett);
    pthread_cancel(hilo);



    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);


    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, 2);
    CU_ASSERT_EQUAL(codop4, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, 3);

    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID1, 0);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID2, 4);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID3, 8);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID4, 12);

    CU_ASSERT_EQUAL(marco1,0);
    CU_ASSERT_EQUAL(marco2,1);
    CU_ASSERT_EQUAL(marco3,2);
    CU_ASSERT_EQUAL(marco4,3);

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos","------------------------------------------");
}


void probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria


    pthread_t hilo = crear_server_botonera_memoria();


    sleep(1);


    uint32_t pid1 = 123;
    uint32_t tamanio1 = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";

    uint32_t  pid2 = 255;
    uint32_t tamanio2 = 1024;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 1024;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 1024;

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%s", "config pasado x parametro: ", ruta_config);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "Pid pasado x parametro: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "tamanio pasado x parametro: ", tamanio1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "Pid pasado x parametro: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "tamanio pasado x parametro: ", tamanio2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "Pid pasado x parametro: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "tamanio pasado x parametro: ", tamanio3);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "Pid pasado x parametro: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "tamanio pasado x parametro: ", tamanio4);

    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid4, tamanio4);

    sleep(1);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid1;
    recv(sockett, &valor_tabla_1er_nivel_pid1, sizeof(uint32_t), false);

    uint32_t codop2;
    recv(sockett, &codop2, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid2;
    recv(sockett, &valor_tabla_1er_nivel_pid2, sizeof(uint32_t), false);

    uint32_t codop3;
    recv(sockett, &codop3, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid3;
    recv(sockett, &valor_tabla_1er_nivel_pid3, sizeof(uint32_t), false);

    uint32_t codop4;
    recv(sockett, &codop4, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid4;
    recv(sockett, &valor_tabla_1er_nivel_pid4, sizeof(uint32_t), false);



    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);


    format_warning_log("test", "valor_tabla_1er_nivel_pid1: %i", valor_tabla_1er_nivel_pid1);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid1, valor_tabla_1er_nivel_pid1, 0);



    uint32_t codopRTA1;
    recv(sockett, &codopRTA1, sizeof(uint32_t), false);

    uint32_t PIDRTA1;
    recv(sockett, &PIDRTA1, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID1;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA1 RECIBIDO: ", codopRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA1 RECIBIDO: ", PIDRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA1 Esperado: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 Esperado: ", 0);



    format_warning_log("test", "valor_tabla_1er_nivel_pid2: %i", valor_tabla_1er_nivel_pid2);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid2, valor_tabla_1er_nivel_pid2, 0);

    uint32_t codopRTA2;
    recv(sockett, &codopRTA2, sizeof(uint32_t), false);

    uint32_t PIDRTA2;
    recv(sockett, &PIDRTA2, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID2;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID2, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA2 RECIBIDO: ", codopRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA2 RECIBIDO: ", PIDRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA2 Esperado: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 Esperado: ", 4);





    format_warning_log("test", "valor_tabla_1er_nivel_pid3: %i", valor_tabla_1er_nivel_pid3);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid3, valor_tabla_1er_nivel_pid3, 0);

    uint32_t codopRTA3;
    recv(sockett, &codopRTA3, sizeof(uint32_t), false);

    uint32_t PIDRTA3;
    recv(sockett, &PIDRTA3, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID3;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID3, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA3 RECIBIDO: ", codopRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA3 RECIBIDO: ", PIDRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA3 Esperado: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 Esperado: ", 8);


    format_warning_log("test", "valor_tabla_1er_nivel_pid4: %i", valor_tabla_1er_nivel_pid4);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid4, valor_tabla_1er_nivel_pid4, 0);

    uint32_t codopRTA4;
    recv(sockett, &codopRTA4, sizeof(uint32_t), false);

    uint32_t PIDRTA4;
    recv(sockett, &PIDRTA4, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID4;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID4, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA4 RECIBIDO: ", codopRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA4 RECIBIDO: ", PIDRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "PIDRTA4 Esperado: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 Esperado: ", 12);


    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE","mando 4 msjs de opcode acceso 2do nivel");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid1, POSICION_TABLA_2DO_NIVEL_PID1, 0);

    uint32_t opcodee1;
    recv(sockett, &opcodee1, sizeof(uint32_t), false);
    uint32_t pidd1;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco1;
    recv(sockett, &marco1, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA1 RECIBIDO: ", opcodee1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd1 RECIBIDO: ", pidd1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd1 Esperado: ", 123);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco RECIBIDO: ", marco1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco Esperado: ", 0);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid2, POSICION_TABLA_2DO_NIVEL_PID2, 0);

    uint32_t opcodee2;
    recv(sockett, &opcodee2, sizeof(uint32_t), false);
    uint32_t pidd2;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco2;
    recv(sockett, &marco2, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA2 RECIBIDO: ", opcodee2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd2 RECIBIDO: ", pidd2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd2 Esperado: ", 255);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco RECIBIDO: ", marco2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco Esperado: ", 1);
    warning_log("----","------------------------------------------------");



    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid3, POSICION_TABLA_2DO_NIVEL_PID3, 0);

    uint32_t opcodee3;
    recv(sockett, &opcodee3, sizeof(uint32_t), false);
    uint32_t pidd3;
    recv(sockett, &pidd3, sizeof(uint32_t), false);
    uint32_t marco3;
    recv(sockett, &marco3, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA3 RECIBIDO: ", opcodee3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd3 RECIBIDO: ", pidd3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd3 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco RECIBIDO: ", marco3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco Esperado: ", 2);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid4, POSICION_TABLA_2DO_NIVEL_PID4, 0);

    uint32_t opcodee4;
    recv(sockett, &opcodee4, sizeof(uint32_t), false);
    uint32_t pidd4;
    recv(sockett, &pidd4, sizeof(uint32_t), false);
    uint32_t marco4;
    recv(sockett, &marco4, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP RTA4 RECIBIDO: ", opcodee4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd4 RECIBIDO: ", pidd4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "pidd4 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco RECIBIDO: ", marco4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "%s%d", "marco Esperado: ", 3);
    warning_log("----","------------------------------------------------");

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "Arranco a probar los WRITE");

    // marco_asignado * datos_traduccion->tamanio_pagina

    int desplazamiento1 = 0;
    uint32_t direccion_fisica_write_1 = marco1 * mem_swap_config->tam_pagina + desplazamiento1;
    uint32_t contenido_write_1 = 12052000;

    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE","ENVIANDO OPCODE_WRITE con PID:%i DF:%i Contenido:%i", pidd1, direccion_fisica_write_1,(int) contenido_write_1 );

    send_codigo_op_con_3_numeros(sockett, OPCODE_WRITE, pidd1, direccion_fisica_write_1, contenido_write_1);

    uint32_t respuesta_write_1;
    recv(sockett, &respuesta_write_1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE",  "Respuesta WRITE RECIBIDA(opcode):%i", (int)respuesta_write_1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE", "Respuesta WRITE Esperada: %i", (int)OPCODE_ACK_OK);
    warning_log("----","------------------------------------------------");

    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte


    cerrar_server_botonera_memoria(sockett);
    desconectar_cliente(sockett);
    pthread_cancel(hilo);



    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);


    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, 2);
    CU_ASSERT_EQUAL(codop4, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, 3);

    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID1, 0);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID2, 4);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID3, 8);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID4, 12);

    CU_ASSERT_EQUAL(marco1,0);
    CU_ASSERT_EQUAL(marco2,1);
    CU_ASSERT_EQUAL(marco3,2);
    CU_ASSERT_EQUAL(marco4,3);

    // Assert write

    CU_ASSERT_EQUAL(respuesta_write_1, OPCODE_ACK_OK);

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_y_OPCODE_ACCESO_2DO_NIVEL_con_varios_procesos","------------------------------------------");
}


void probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria


    pthread_t hilo = crear_server_botonera_memoria();


    sleep(1);


    uint32_t pid1 = 123;
    uint32_t tamanio1 = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";

    uint32_t  pid2 = 255;
    uint32_t tamanio2 = 1024;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 1024;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 1024;

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%s", "config pasado x parametro: ", ruta_config);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio3);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio4);

    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid4, tamanio4);

    sleep(1);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid1;
    recv(sockett, &valor_tabla_1er_nivel_pid1, sizeof(uint32_t), false);

    uint32_t codop2;
    recv(sockett, &codop2, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid2;
    recv(sockett, &valor_tabla_1er_nivel_pid2, sizeof(uint32_t), false);

    uint32_t codop3;
    recv(sockett, &codop3, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid3;
    recv(sockett, &valor_tabla_1er_nivel_pid3, sizeof(uint32_t), false);

    uint32_t codop4;
    recv(sockett, &codop4, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid4;
    recv(sockett, &valor_tabla_1er_nivel_pid4, sizeof(uint32_t), false);



    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);


    format_warning_log("test", "valor_tabla_1er_nivel_pid1: %i", valor_tabla_1er_nivel_pid1);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid1, valor_tabla_1er_nivel_pid1, 0);



    uint32_t codopRTA1;
    recv(sockett, &codopRTA1, sizeof(uint32_t), false);

    uint32_t PIDRTA1;
    recv(sockett, &PIDRTA1, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID1;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA1 RECIBIDO: ", codopRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA1 RECIBIDO: ", PIDRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA1 Esperado: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 Esperado: ", 0);



    format_warning_log("test", "valor_tabla_1er_nivel_pid2: %i", valor_tabla_1er_nivel_pid2);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid2, valor_tabla_1er_nivel_pid2, 0);

    uint32_t codopRTA2;
    recv(sockett, &codopRTA2, sizeof(uint32_t), false);

    uint32_t PIDRTA2;
    recv(sockett, &PIDRTA2, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID2;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID2, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA2 RECIBIDO: ", codopRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA2 RECIBIDO: ", PIDRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA2 Esperado: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 Esperado: ", 4);





    format_warning_log("test", "valor_tabla_1er_nivel_pid3: %i", valor_tabla_1er_nivel_pid3);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid3, valor_tabla_1er_nivel_pid3, 0);

    uint32_t codopRTA3;
    recv(sockett, &codopRTA3, sizeof(uint32_t), false);

    uint32_t PIDRTA3;
    recv(sockett, &PIDRTA3, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID3;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID3, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA3 RECIBIDO: ", codopRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA3 RECIBIDO: ", PIDRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA3 Esperado: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 Esperado: ", 8);


    format_warning_log("test", "valor_tabla_1er_nivel_pid4: %i", valor_tabla_1er_nivel_pid4);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid4, valor_tabla_1er_nivel_pid4, 0);

    uint32_t codopRTA4;
    recv(sockett, &codopRTA4, sizeof(uint32_t), false);

    uint32_t PIDRTA4;
    recv(sockett, &PIDRTA4, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID4;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID4, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA4 RECIBIDO: ", codopRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA4 RECIBIDO: ", PIDRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "PIDRTA4 Esperado: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 Esperado: ", 12);


    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ","mando 4 msjs de opcode acceso 2do nivel");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid1, POSICION_TABLA_2DO_NIVEL_PID1, 0);

    uint32_t opcodee1;
    recv(sockett, &opcodee1, sizeof(uint32_t), false);
    uint32_t pidd1;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco1;
    recv(sockett, &marco1, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA1 RECIBIDO: ", opcodee1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd1 RECIBIDO: ", pidd1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd1 Esperado: ", 123);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco Esperado: ", 0);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid2, POSICION_TABLA_2DO_NIVEL_PID2, 0);

    uint32_t opcodee2;
    recv(sockett, &opcodee2, sizeof(uint32_t), false);
    uint32_t pidd2;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco2;
    recv(sockett, &marco2, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA2 RECIBIDO: ", opcodee2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd2 RECIBIDO: ", pidd2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd2 Esperado: ", 255);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco Esperado: ", 1);
    warning_log("----","------------------------------------------------");



    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid3, POSICION_TABLA_2DO_NIVEL_PID3, 0);

    uint32_t opcodee3;
    recv(sockett, &opcodee3, sizeof(uint32_t), false);
    uint32_t pidd3;
    recv(sockett, &pidd3, sizeof(uint32_t), false);
    uint32_t marco3;
    recv(sockett, &marco3, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA3 RECIBIDO: ", opcodee3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd3 RECIBIDO: ", pidd3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd3 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco Esperado: ", 2);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid4, POSICION_TABLA_2DO_NIVEL_PID4, 0);

    uint32_t opcodee4;
    recv(sockett, &opcodee4, sizeof(uint32_t), false);
    uint32_t pidd4;
    recv(sockett, &pidd4, sizeof(uint32_t), false);
    uint32_t marco4;
    recv(sockett, &marco4, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP RTA4 RECIBIDO: ", opcodee4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd4 RECIBIDO: ", pidd4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "pidd4 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "%s%d", "marco Esperado: ", 3);
    warning_log("----","------------------------------------------------");

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "Arranco a probar los WRITE");

    // marco_asignado * datos_traduccion->tamanio_pagina

    int desplazamiento1 = 0;
    uint32_t direccion_fisica_write_1 = marco1 * mem_swap_config->tam_pagina + desplazamiento1;
    uint32_t contenido_write_1 = 12052000;

    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ","ENVIANDO OPCODE_WRITE con PID:%i DF:%i Contenido:%i", pidd1, direccion_fisica_write_1,(int) contenido_write_1 );

    send_codigo_op_con_3_numeros(sockett, OPCODE_WRITE, pidd1, direccion_fisica_write_1, contenido_write_1);

    uint32_t respuesta_write_1;
    recv(sockett, &respuesta_write_1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ",  "Respuesta WRITE RECIBIDA(opcode):%i", (int)respuesta_write_1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "Respuesta WRITE Esperada: %i", (int)OPCODE_ACK_OK);
    warning_log("----","------------------------------------------------");


    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ","ENVIANDO READ con PID:%i DF:%i", pidd1, direccion_fisica_write_1 );

    send_codigo_op_con_numeros(sockett, OPCODE_READ, pidd1, direccion_fisica_write_1);


    uint32_t opcode_rta_1;
    recv(sockett, &opcode_rta_1, sizeof(uint32_t), false);
    uint32_t pid_rta_1;
    recv(sockett, &pid_rta_1, sizeof(uint32_t), false);
    uint32_t lectura_1;
    recv(sockett, &lectura_1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ",  "Respuesta READ RECIBIDA Opcode:%i PID:%i Contenido: %i", (int)opcode_rta_1, (int)pid_rta_1, (int)lectura_1) ;
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ", "Respuesta READ Esperada: %i PID:%i Contenido: %i", (int)OPCODE_READ,(int) pidd1, 12052000 );
    warning_log("----","------------------------------------------------");




    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte


    cerrar_server_botonera_memoria(sockett);
    desconectar_cliente(sockett);
    pthread_cancel(hilo);



    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);


    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, 2);
    CU_ASSERT_EQUAL(codop4, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, 3);

    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID1, 0);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID2, 4);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID3, 8);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID4, 12);

    CU_ASSERT_EQUAL(marco1,0);
    CU_ASSERT_EQUAL(marco2,1);
    CU_ASSERT_EQUAL(marco3,2);
    CU_ASSERT_EQUAL(marco4,3);

    // Assert write

    CU_ASSERT_EQUAL(respuesta_write_1, OPCODE_ACK_OK);

    // Assert READ

    CU_ASSERT_EQUAL(opcode_rta_1, OPCODE_READ);
    CU_ASSERT_EQUAL(pid_rta_1, pidd1);
    CU_ASSERT_EQUAL(lectura_1, 12052000);

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_OPCODE_WRITE_y_OPCODE_READ","------------------------------------------");
}




void probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ() {
    pthread_mutex_lock(&sum_mutex);
    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","------------------------------------------");
    // Iniciamos un hilo detached que corra el servidor de memoria


    pthread_t hilo = crear_server_botonera_memoria();


    sleep(1);


    uint32_t pid1 = 123;
    uint32_t tamanio1 = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";

    uint32_t  pid2 = 255;
    uint32_t tamanio2 = 1024;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 1024;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 1024;

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%s", "config pasado x parametro: ", ruta_config);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio3);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "Pid pasado x parametro: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "tamanio pasado x parametro: ", tamanio4);

    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, OPCODE_VALUE_TAB_PAG, pid4, tamanio4);

    sleep(1);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid1;
    recv(sockett, &valor_tabla_1er_nivel_pid1, sizeof(uint32_t), false);

    uint32_t codop2;
    recv(sockett, &codop2, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid2;
    recv(sockett, &valor_tabla_1er_nivel_pid2, sizeof(uint32_t), false);

    uint32_t codop3;
    recv(sockett, &codop3, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid3;
    recv(sockett, &valor_tabla_1er_nivel_pid3, sizeof(uint32_t), false);

    uint32_t codop4;
    recv(sockett, &codop4, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel_pid4;
    recv(sockett, &valor_tabla_1er_nivel_pid4, sizeof(uint32_t), false);



    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);


    format_warning_log("test", "valor_tabla_1er_nivel_pid1: %i", valor_tabla_1er_nivel_pid1);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid1, valor_tabla_1er_nivel_pid1, 0);



    uint32_t codopRTA1;
    recv(sockett, &codopRTA1, sizeof(uint32_t), false);

    uint32_t PIDRTA1;
    recv(sockett, &PIDRTA1, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID1;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA1 RECIBIDO: ", codopRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA1 RECIBIDO: ", PIDRTA1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA1 Esperado: ", pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID1 Esperado: ", 0);



    format_warning_log("test", "valor_tabla_1er_nivel_pid2: %i", valor_tabla_1er_nivel_pid2);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid2, valor_tabla_1er_nivel_pid2, 0);

    uint32_t codopRTA2;
    recv(sockett, &codopRTA2, sizeof(uint32_t), false);

    uint32_t PIDRTA2;
    recv(sockett, &PIDRTA2, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID2;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID2, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA2 RECIBIDO: ", codopRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA2 RECIBIDO: ", PIDRTA2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA2 Esperado: ", pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 Esperado: ", 4);





    format_warning_log("test", "valor_tabla_1er_nivel_pid3: %i", valor_tabla_1er_nivel_pid3);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid3, valor_tabla_1er_nivel_pid3, 0);

    uint32_t codopRTA3;
    recv(sockett, &codopRTA3, sizeof(uint32_t), false);

    uint32_t PIDRTA3;
    recv(sockett, &PIDRTA3, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID3;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID3, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA3 RECIBIDO: ", codopRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA3 RECIBIDO: ", PIDRTA3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA3 Esperado: ", pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 Esperado: ", 8);


    format_warning_log("test", "valor_tabla_1er_nivel_pid4: %i", valor_tabla_1er_nivel_pid4);
    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_1ER_NIVEL, pid4, valor_tabla_1er_nivel_pid4, 0);

    uint32_t codopRTA4;
    recv(sockett, &codopRTA4, sizeof(uint32_t), false);

    uint32_t PIDRTA4;
    recv(sockett, &PIDRTA4, sizeof(uint32_t), false);

    uint32_t POSICION_TABLA_2DO_NIVEL_PID4;
    recv(sockett, &POSICION_TABLA_2DO_NIVEL_PID4, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA4 RECIBIDO: ", codopRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_1ER_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA4 RECIBIDO: ", PIDRTA4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "PIDRTA4 Esperado: ", pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 RECIBIDO: ", POSICION_TABLA_2DO_NIVEL_PID4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 Esperado: ", 12);


    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","mando 4 msjs de opcode acceso 2do nivel");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid1, POSICION_TABLA_2DO_NIVEL_PID1, 0);

    uint32_t opcodee1;
    recv(sockett, &opcodee1, sizeof(uint32_t), false);
    uint32_t pidd1;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco1;
    recv(sockett, &marco1, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA1 RECIBIDO: ", opcodee1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd1 RECIBIDO: ", pidd1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd1 Esperado: ", 123);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco Esperado: ", 0);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid2, POSICION_TABLA_2DO_NIVEL_PID2, 0);

    uint32_t opcodee2;
    recv(sockett, &opcodee2, sizeof(uint32_t), false);
    uint32_t pidd2;
    recv(sockett, &pidd1, sizeof(uint32_t), false);
    uint32_t marco2;
    recv(sockett, &marco2, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA2 RECIBIDO: ", opcodee2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd2 RECIBIDO: ", pidd2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd2 Esperado: ", 255);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco Esperado: ", 1);
    warning_log("----","------------------------------------------------");



    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid3, POSICION_TABLA_2DO_NIVEL_PID3, 0);

    uint32_t opcodee3;
    recv(sockett, &opcodee3, sizeof(uint32_t), false);
    uint32_t pidd3;
    recv(sockett, &pidd3, sizeof(uint32_t), false);
    uint32_t marco3;
    recv(sockett, &marco3, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA3 RECIBIDO: ", opcodee3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd3 RECIBIDO: ", pidd3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd3 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco Esperado: ", 2);
    warning_log("----","------------------------------------------------");

    send_codigo_op_con_3_numeros(sockett, OPCODE_ACCESO_2DO_NIVEL, pid4, POSICION_TABLA_2DO_NIVEL_PID4, 0);

    uint32_t opcodee4;
    recv(sockett, &opcodee4, sizeof(uint32_t), false);
    uint32_t pidd4;
    recv(sockett, &pidd4, sizeof(uint32_t), false);
    uint32_t marco4;
    recv(sockett, &marco4, sizeof(uint32_t), false);

    warning_log("----","------------------------------------------------");
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP RTA4 RECIBIDO: ", opcodee4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "CODOP esperado: ", OPCODE_ACCESO_2DO_NIVEL);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd4 RECIBIDO: ", pidd4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "pidd4 Esperado: ", 128);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco RECIBIDO: ", marco4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "%s%d", "marco Esperado: ", 3);
    warning_log("----","------------------------------------------------");

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "Arranco a probar los WRITE");

    // marco_asignado * datos_traduccion->tamanio_pagina

    int desplazamiento1 = 0;
    uint32_t direccion_fisica_write_1 = marco1 * mem_swap_config->tam_pagina + desplazamiento1;
    uint32_t contenido_write_1 = 12052000;

    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","ENVIANDO OPCODE_WRITE con PID:%i DF:%i Contenido:%i", pidd1, direccion_fisica_write_1,(int) contenido_write_1 );

    send_codigo_op_con_3_numeros(sockett, OPCODE_WRITE, pidd1, direccion_fisica_write_1, contenido_write_1);

    uint32_t respuesta_write_1;
    recv(sockett, &respuesta_write_1, sizeof(uint32_t), false);



    int desplazamiento2 = 0;
    uint32_t direccion_fisica_write_2 = marco2 * mem_swap_config->tam_pagina + desplazamiento1;
    uint32_t contenido_write_2 = 222222;

    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","ENVIANDO OPCODE_WRITE con PID:%i DF:%i Contenido:%i", pidd2, direccion_fisica_write_2,(int) contenido_write_2 );

    send_codigo_op_con_3_numeros(sockett, OPCODE_WRITE, pidd2, direccion_fisica_write_2, contenido_write_2);

    uint32_t respuesta_write_2;
    recv(sockett, &respuesta_write_2, sizeof(uint32_t), false);



    int desplazamiento3 = 0;
    uint32_t direccion_fisica_write_3 = marco3 * mem_swap_config->tam_pagina + desplazamiento1;
    uint32_t contenido_write_3 = 333333;

    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","ENVIANDO OPCODE_WRITE con PID:%i DF:%i Contenido:%i", pidd3, direccion_fisica_write_3,(int) contenido_write_3 );

    send_codigo_op_con_3_numeros(sockett, OPCODE_WRITE, pidd3, direccion_fisica_write_3, contenido_write_3);

    uint32_t respuesta_write_3;
    recv(sockett, &respuesta_write_3, sizeof(uint32_t), false);



    int desplazamiento4 = 0;
    uint32_t direccion_fisica_write_4 = marco4 * mem_swap_config->tam_pagina + desplazamiento1;
    uint32_t contenido_write_4 = 444444;

    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","ENVIANDO OPCODE_WRITE con PID:%i DF:%i Contenido:%i", pidd4, direccion_fisica_write_4,(int) contenido_write_4 );

    send_codigo_op_con_3_numeros(sockett, OPCODE_WRITE, pidd4, direccion_fisica_write_4, contenido_write_4);

    uint32_t respuesta_write_4;
    recv(sockett, &respuesta_write_4, sizeof(uint32_t), false);





    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ",  "Respuesta WRITE RECIBIDA(opcode):%i", (int)respuesta_write_1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "Respuesta WRITE Esperada: %i", (int)OPCODE_ACK_OK);
    warning_log("----","------------------------------------------------");


    format_warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","ENVIANDO READ con PID:%i DF:%i", pidd1, direccion_fisica_write_1 );

    send_codigo_op_con_numeros(sockett, OPCODE_READ, pidd1, direccion_fisica_write_1);


    uint32_t opcode_rta_1;
    recv(sockett, &opcode_rta_1, sizeof(uint32_t), false);
    uint32_t pid_rta_1;
    recv(sockett, &pid_rta_1, sizeof(uint32_t), false);
    uint32_t lectura_1;
    recv(sockett, &lectura_1, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ",  "Respuesta READ RECIBIDA Opcode:%i PID:%i Contenido: %i", (int)opcode_rta_1, (int)pid_rta_1, (int)lectura_1) ;
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "Respuesta READ Esperada: %i PID:%i Contenido: %i", (int)OPCODE_READ,(int) pidd1, 12052000 );


    send_codigo_op_con_numeros(sockett, OPCODE_READ, pidd2, direccion_fisica_write_2);

    uint32_t opcode_rta_2;
    recv(sockett, &opcode_rta_2, sizeof(uint32_t), false);
    uint32_t pid_rta_2;
    recv(sockett, &pid_rta_2, sizeof(uint32_t), false);
    uint32_t lectura_2;
    recv(sockett, &lectura_2, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ",  "Respuesta READ RECIBIDA Opcode:%i PID:%i Contenido: %i", (int)opcode_rta_2, (int)pid_rta_2, (int)lectura_2) ;
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "Respuesta READ Esperada: %i PID:%i Contenido: %i", (int)OPCODE_READ,(int) pidd2, 222222 );

    send_codigo_op_con_numeros(sockett, OPCODE_READ, pidd3, direccion_fisica_write_3);

    uint32_t opcode_rta_3;
    recv(sockett, &opcode_rta_3, sizeof(uint32_t), false);
    uint32_t pid_rta_3;
    recv(sockett, &pid_rta_3, sizeof(uint32_t), false);
    uint32_t lectura_3;
    recv(sockett, &lectura_3, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ",  "Respuesta READ RECIBIDA Opcode:%i PID:%i Contenido: %i", (int)opcode_rta_3, (int)pid_rta_3, (int)lectura_3) ;
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "Respuesta READ Esperada: %i PID:%i Contenido: %i", (int)OPCODE_READ,(int) pidd3, 333333 );

    send_codigo_op_con_numeros(sockett, OPCODE_READ, pidd4, direccion_fisica_write_4);

    uint32_t opcode_rta_4;
    recv(sockett, &opcode_rta_4, sizeof(uint32_t), false);
    uint32_t pid_rta_4;
    recv(sockett, &pid_rta_4, sizeof(uint32_t), false);
    uint32_t lectura_4;
    recv(sockett, &lectura_4, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ",  "Respuesta READ RECIBIDA Opcode:%i PID:%i Contenido: %i", (int)opcode_rta_4, (int)pid_rta_4, (int)lectura_4) ;
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ", "Respuesta READ Esperada: %i PID:%i Contenido: %i", (int)OPCODE_READ,(int) pidd4, 444444 );



    warning_log("----","------------------------------------------------");




    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    desconectar_cliente(sockett);

    cerrar_server_botonera_memoria(sockett);
    pthread_cancel(hilo);



    // Finalmente hacemos los asserts
    pthread_mutex_unlock(&sum_mutex);


    CU_ASSERT_EQUAL(codop, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, 2);
    CU_ASSERT_EQUAL(codop4, OPCODE_VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, 3);

    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID1, 0);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID2, 4);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID3, 8);
    CU_ASSERT_EQUAL(POSICION_TABLA_2DO_NIVEL_PID4, 12);

    CU_ASSERT_EQUAL(marco1,0);
    CU_ASSERT_EQUAL(marco2,1);
    CU_ASSERT_EQUAL(marco3,2);
    CU_ASSERT_EQUAL(marco4,3);

    // Assert write

    CU_ASSERT_EQUAL(respuesta_write_1, OPCODE_ACK_OK);
    CU_ASSERT_EQUAL(respuesta_write_2, OPCODE_ACK_OK);
    CU_ASSERT_EQUAL(respuesta_write_3, OPCODE_ACK_OK);
    CU_ASSERT_EQUAL(respuesta_write_4, OPCODE_ACK_OK);


    // Assert READ

    CU_ASSERT_EQUAL(opcode_rta_1, OPCODE_READ);
    CU_ASSERT_EQUAL(pid_rta_1, pidd1);
    CU_ASSERT_EQUAL(lectura_1, 12052000);

    CU_ASSERT_EQUAL(opcode_rta_2, OPCODE_READ);
    CU_ASSERT_EQUAL(pid_rta_2, pidd2);
    CU_ASSERT_EQUAL(lectura_2, 222222);


    CU_ASSERT_EQUAL(opcode_rta_3, OPCODE_READ);
    CU_ASSERT_EQUAL(pid_rta_3, pidd3);
    CU_ASSERT_EQUAL(lectura_3, 333333);

    CU_ASSERT_EQUAL(opcode_rta_4, OPCODE_READ);
    CU_ASSERT_EQUAL(pid_rta_4, pidd4);
    CU_ASSERT_EQUAL(lectura_4, 444444);



    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos_y_OPCODE_ACCESO_2DO_NIVEL_y_VARIOS_OPCODE_WRITE_y_VARIOS_OPCODE_READ","------------------------------------------");
}

