#include "main.h"
#include <parser.h>



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

    puts("Hello world!!");

    t_log* logger = log_create("./log.txt", "Consola", 1, LOG_LEVEL_INFO);
    
    log_debug(logger, "DEBUG LOG!");
    log_info(logger, "INFO LOG!");
    log_error(logger, "PRUEBA!");

    //hello_world(); perdon luis pero no anda mas el hello world :(

    iniciar_config_consola("../config/ejemplo.config");
    print_config();

    destruir_estructura_consola_config();
    log_destroy(logger);

    return 0;
}
