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
    uint32_t tamanio3 = 2048;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 4096;

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

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", OPCODE_VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);




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
    uint32_t tamanio2 = 512;

    uint32_t pid3 = 128;
    uint32_t tamanio3 = 2048;

    uint32_t pid4 = 44;
    uint32_t tamanio4 = 4096;

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
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID2 Esperado: ", 1);





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
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID3 Esperado: ", 1);


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
    format_info_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos", "%s%d", "POSICION_TABLA_2DO_NIVEL_PID4 Esperado: ", 1);






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

    warning_log("botonera.c@probar_VALUE_TAB_PAG_y_OPCODE_ACCESO_1ER_NIVEL_con_varios_procesos","------------------------------------------");
}
