#include "../include/main.h"

/*
    Para ejecutar la consola, el binario deberá recibir 3 argumentos
        * Ruta al archivo de instrucciones
        * Tamaño del proceso
        * Ruta al archivo de config

    ej:
    $ cd bin
    $ ./consola.out ../instrucciones/ejemplo.txt 123 ../config/ejemplo.config
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

	printf("%s", path_instruction);
	printf("%d", tamanio_proceso);
	printf("%s", path_arch_config);
	
	t_list* lista_de_instrucciones;

	lista_de_instrucciones = config_create_parser(path_instruction);

	void printear_lista(instruccion_t* una_instruccion){

		printf("%d %d %d\n", una_instruccion->instruccion, una_instruccion->argumentos[0], una_instruccion->argumentos[1]);

	};

	list_iterate(lista_de_instrucciones, (void*)printear_lista);

	uint32_t codop = 100; // TODO

	//SERIALIZAR
	void* stream = serializar_lista_de_instrucciones(lista_de_instrucciones, codop);

	//DESERIALIZAR
	t_list* instrucciones_deserializadas = deserializar_lista_de_instrucciones(stream);

	free(stream);

	list_destroy_and_destroy_elements(instrucciones_deserializadas, (void*)free);
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void*)free);

    cerrar_consola();

    return 0;
}
