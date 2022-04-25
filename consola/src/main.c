#include "../include/main.h"

/*
    Para ejecutar la consola, make start (args en settings.mk) o sino el binario deberá recibir 3 argumentos
        * Ruta al archivo de instrucciones
        * Tamaño del proceso
        * Ruta al archivo de config

    ej:
    $ cd bin
    $ ./consola.out ../instrucciones/instrucciones.txt 123 ../config/ejemplo.config
*/
int main(int argc /*cantidad de argumentos*/, char* argv[]/*path instruccion +tamanio proceso+ arch config */) {

    setup_logger();

    hello_world();

    iniciar_config_consola(argv[3]);
    print_config();

	char* path_instruction;
	int tamanio_proceso;
	char* path_arch_config;

	path_instruction = argv[1];
	tamanio_proceso = atoi(argv[2]);
	path_arch_config = argv[3];
    info_log("main.c@main", "Path de instrucciones");
	info_log("main.c@main", path_instruction);
    info_log("main.c@main", "Tamaño del proceso");
    info_log("main.c@main", argv[2]);
    info_log("main.c@main", "Path del archivo de configuración");
    info_log("main.c@main", path_arch_config);

	t_list* lista_de_instrucciones = config_create_parser(path_instruction);

	list_iterate(lista_de_instrucciones, (void*) &printear_instruccion);

    enviar_proceso_a_kernel(lista_de_instrucciones);

	//DESERIALIZAR
	//t_list* instrucciones_deserializadas = deserializar_lista_de_instrucciones(stream);

	//list_destroy_and_destroy_elements(instrucciones_deserializadas, (void*)&free);
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void*)&free);

    cerrar_consola();

    return 0;
}
