#include "../include/serializacion.h"



void* serializar_lista_de_instrucciones (t_list* lista_de_instrucciones , uint32_t codop){

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



	return stream;
}


t_list* deserializar_lista_de_instrucciones (void* stream){


	//DESARMAR STREAM
	int desplazamiento=0;
	t_list* instrucciones_deserializadas = list_create();
	uint32_t codop_deserializado;
	uint32_t list_size_deserializado;


	//CODOP TODO
	memcpy(&codop_deserializado, stream, sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

	memcpy(&list_size_deserializado, stream+desplazamiento, sizeof(uint32_t));
	desplazamiento+=sizeof(uint32_t);

	printf("El CODOP ES: %i \n" , codop_deserializado);
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


	return instrucciones_deserializadas;

}
