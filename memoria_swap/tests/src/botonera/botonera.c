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

void probar_VALUE_TAB_PAG(uint32_t pid, uint32_t tamanio, char* ruta_config) {
/*
    void* mensaje = malloc( sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t));
    uint32_t codop = (uint32_t) VALUE_TAB_PAG;
    memcpy(mensaje, &codop, sizeof (uint32_t));
    memcpy(mensaje + sizeof(uint32_t), &pid, sizeof (uint32_t));
    memcpy(mensaje + sizeof(uint32_t) + sizeof(uint32_t), &tamanio, sizeof (uint32_t));
*/
    pid = 123;
    tamanio = 1024;
    ruta_config = "./configs/memoria/memoria.config";


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

    desconectar_cliente(sockett);

    CU_ASSERT_EQUAL(codop, VALUE_TAB_PAG);
    CU_ASSERT_EQUAL(valor_tabla_1er_nivel, 0);





    //liberar_conexion(sockett, "tests/botonera.c");

    //CU_ASSERT_STRING_EQUAL(mem_swap_config->ip_escucha, "127.0.0.1");

}
