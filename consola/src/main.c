#include "../include/main.h"


int main(int argc /*cantidad de argumentos*/, char* argv[]/*path instruccion +tamanio proceso+ arch config */) {

	char* path_instruction;
	int tamanio_proceso;
	char* path_arch_config;

	path_instruction = argv[1];
	tamanio_proceso = atoi(argv[2]);
	path_arch_config = argv[3];

	printf("%s", path_instruction);
	printf("%d", tamanio_proceso);
	printf("%s", path_arch_config);
	
	t_list* lista_de_instrucciones;

	lista_de_instrucciones = config_create_parser(path_instruction);

	void printear_lista(instruccion_t* una_instruccion){

		printf("%d %d %d\n", una_instruccion->instruccion, una_instruccion->argumentos[0], una_instruccion->argumentos[1]);

	};

	list_iterate(lista_de_instrucciones, (void*)printear_lista);
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void*)free);

    

    // Se hace una sola vez a nivel aplicacion
    logger_set_module("CONSOLA");
    logger_set_loglevel(LOG_LEVEL_DEBUG);
    info_log("main.c", "Bienvenido a CONSOLA!!!");

    debug_log("main.c", "DEBUG LOG!");
    info_log("main.c", "INFO LOG!");
    error_log("main.c", "PRUEBA!");

    hello_world();

    iniciar_config_consola("./ejemplo.config");
    print_config();
    info_log("main.c", "Se comienza a cerrar aplicacion CONSOLA ");
    destruir_estructura_consola_config();

    return 0;
}
