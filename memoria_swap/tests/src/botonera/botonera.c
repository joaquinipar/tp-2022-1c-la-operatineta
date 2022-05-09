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

    // Iniciamos un hilo detached que corra el servidor de memoria
    pthread_t hilo =crear_server_botonera_memoria();

    uint32_t pid = 12344;
    uint32_t tamanio = 1024;
    char* ruta_config = "./configs/memoria/memoria.config";


    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "Pid pasado x parametro: ", pid);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "tamanio pasado x parametro: ", tamanio);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%s", "config pasado x parametro: ", ruta_config);


    int sockett = iniciar_cliente_test(2, ruta_config);


    send_codigo_op_con_numeros(sockett, VALUE_TAB_PAG, pid, tamanio);

    uint32_t codop;
    recv(sockett, &codop, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel;
    recv(sockett, &valor_tabla_1er_nivel, sizeof(uint32_t), false);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);


    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    cerrar_server_botonera_memoria(sockett);

    desconectar_cliente(sockett);

    pthread_cancel(hilo);

    // Finalmente hacemos los asserts

    CU_ASSERT_EQUAL(codop, VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel, 0);
}


void probar_VALUE_TAB_PAG_con_varios_procesos() {

    // Iniciamos un hilo detached que corra el servidor de memoria
    pthread_t hilo = crear_server_botonera_memoria();

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


    send_codigo_op_con_numeros(sockett, VALUE_TAB_PAG, pid1, tamanio1);
    send_codigo_op_con_numeros(sockett, VALUE_TAB_PAG, pid2, tamanio2);
    send_codigo_op_con_numeros(sockett, VALUE_TAB_PAG, pid3, tamanio3);
    send_codigo_op_con_numeros(sockett, VALUE_TAB_PAG, pid4, tamanio4);

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
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid1);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 0);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid2);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 1);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid3);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 2);

    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP RECIBIDO: ", codop4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "CODOP esperado: ", VALUE_TAB_PAG);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL RECIBIDO: ", valor_tabla_1er_nivel_pid4);
    format_info_log("botonera.c@probar_VALUE_TAB_PAG", "%s%d", "TABLA_1ER_NIVEL Esperado: ", 3);




    // Enviamos OPCODE __ABORT__ para que el servidor de memoria aborte
    cerrar_server_botonera_memoria(sockett);

    desconectar_cliente(sockett);
    pthread_cancel(hilo);
    // Finalmente hacemos los asserts

    CU_ASSERT_EQUAL(codop, VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid1, 0);
    CU_ASSERT_EQUAL(codop2, VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid2, 1);
    CU_ASSERT_EQUAL(codop3, VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid3, 2);
    CU_ASSERT_EQUAL(codop4, VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel_pid4, 3);
}
