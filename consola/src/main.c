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



	uint32_t codop = 100; // TODO
	void* stream;
	//CODOP + LIST_SIZE + (UINT32 + INT32 + INT32)*


	uint32_t cantidad_lista_instrucciones = (uint32_t) list_size(lista_de_instrucciones);
	uint32_t tamanio_codop = sizeof(uint32_t);
	uint32_t tamanio_instruccion = sizeof(uint32_t) + sizeof(int32_t) + sizeof(int32_t);


	stream = malloc(tamanio_codop + cantidad_lista_instrucciones + (cantidad_lista_instrucciones * tamanio_instruccion));

	int desplazamiento = 0;

	memcpy(stream, &codop , tamanio_codop);
	desplazamiento+= sizeof(codop);

	memcpy(stream + desplazamiento, &cantidad_lista_instrucciones , sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);



	void cargar_instruccion_a_stream(instruccion_t* una_instruccion){

		memcpy(stream + desplazamiento,&(una_instruccion->instruccion),sizeof(uint32_t) );
		desplazamiento+= sizeof(uint32_t);

		memcpy(stream + desplazamiento,&(una_instruccion->argumentos[0]),sizeof(int32_t) );
		desplazamiento+= sizeof(int32_t);

		memcpy(stream + desplazamiento,&(una_instruccion->argumentos[1]),sizeof(int32_t) );
		desplazamiento+= sizeof(int32_t);

	};

	list_iterate(lista_de_instrucciones, (void*)cargar_instruccion_a_stream);





	//DESARMAR STREAM
	desplazamiento=0;
	t_list* instrucciones_deserializadas = list_create();
	uint32_t codop_deserializado;
	uint32_t list_size_deserializado;

	memcpy(&codop_deserializado, stream, sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

	memcpy(&list_size_deserializado, stream+desplazamiento, sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

	printf("El CODOP ES: %i \n" , codop);
	printf("El LIST_SIZE ES: %i \n" , list_size_deserializado);


	for(int i=0; i< list_size_deserializado ; i++){

		instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));

		memcpy(&(instruccion_a_cargar->instruccion),stream + desplazamiento ,sizeof(uint32_t)  ) ;
		printf("La instruccion tiene valor ENUM: %i \n" , instruccion_a_cargar->instruccion);
		desplazamiento+=sizeof(uint32_t);


		memcpy(&(instruccion_a_cargar->argumentos[0]),stream + desplazamiento ,sizeof(int32_t) ) ;
		printf("La instruccion tiene parametro 0 : %i \n" , instruccion_a_cargar->argumentos[0]);
		desplazamiento+=sizeof(int32_t);

		memcpy(&(instruccion_a_cargar->argumentos[1]),stream + desplazamiento ,sizeof(int32_t) ) ;
		printf("La instruccion tiene parametro 1 : %i \n" , instruccion_a_cargar->argumentos[1]);
		desplazamiento+=sizeof(int32_t);


		list_add( instrucciones_deserializadas, instruccion_a_cargar);

	}




	free(stream);

	list_destroy_and_destroy_elements(instrucciones_deserializadas, (void*)free);
	list_destroy_and_destroy_elements(lista_de_instrucciones, (void*)free);





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
