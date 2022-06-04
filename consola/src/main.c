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

	char* path_instruction = argv[1];
	int tamanio_proceso = atoi(argv[2]);
	char* path_arch_config = argv[3];;

    format_info_log("main.c@main", "Path de instrucciones: %s", path_instruction);
    format_info_log("main.c@main", "Tamaño del proceso: %d", tamanio_proceso);
    format_info_log("main.c@main", "Path del archivo de configuración: %s", path_arch_config);

	debug_log("main.c@main", "Parseando lista de instrucciones");
    t_list* lista_de_instrucciones = config_create_parser(path_instruction);

	debug_log("main.c@main", "Imprimiendo lista de instrucciones");
    list_iterate(lista_de_instrucciones, (void*) &printear_instruccion);

    
	debug_log("main.c@main", "Enviando lista de instrucciones a kernel");
    enviar_proceso_a_kernel(lista_de_instrucciones, tamanio_proceso);

	//DESERIALIZAR
	//t_list* instrucciones_deserializadas = deserializar_lista_de_instrucciones(stream);

	//list_destroy_and_destroy_elements(instrucciones_deserializadas, (void*)&free);
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void*)&free);

    cerrar_consola();

    return 0;
}
