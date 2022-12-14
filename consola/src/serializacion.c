#include "../include/serializacion.h"

uint32_t serializar_lista_de_instrucciones (t_list* lista_de_instrucciones , op_code_t codop, void **stream, uint32_t tamanio) {

	//CODOP + LIST_SIZE + (UINT32 + INT32 + INT32)*

	uint32_t cantidad_lista_instrucciones = (uint32_t) list_size(lista_de_instrucciones);
	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "Cant List Instr: %d", cantidad_lista_instrucciones); 
	//uint32_t tamanio_codop = sizeof(uint32_t);
	uint32_t tamanio_instruccion = sizeof(uint32_t) + sizeof(int32_t) + sizeof(int32_t);
	uint32_t tamanio_stream = sizeof(op_code_t) + cantidad_lista_instrucciones + (cantidad_lista_instrucciones * tamanio_instruccion * sizeof(uint32_t)) + sizeof(uint32_t);

	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "Tamaño del proceso: %d", tamanio); 
	*stream = malloc(tamanio_stream);

	int desplazamiento = 0;

	memcpy(*stream, &codop , sizeof(op_code_t));
	desplazamiento+= sizeof(op_code_t);

	memcpy(*stream + desplazamiento, &cantidad_lista_instrucciones , sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	void cargar_instruccion_a_stream(instruccion_t* una_instruccion) {

		memcpy(*stream + desplazamiento,&(una_instruccion->instruccion),sizeof(uint32_t) );
		desplazamiento+= sizeof(uint32_t);

		memcpy(*stream + desplazamiento,&(una_instruccion->argumentos[0]),sizeof(int32_t) );
		desplazamiento+= sizeof(int32_t);

		memcpy(*stream + desplazamiento,&(una_instruccion->argumentos[1]),sizeof(int32_t) );
		desplazamiento+= sizeof(int32_t);

	};

	list_iterate(lista_de_instrucciones, (void*)cargar_instruccion_a_stream);


	memcpy(*stream + desplazamiento, &tamanio , sizeof(uint32_t));
		//desplazamiento+= sizeof(op_code_t);

	return tamanio_stream;
}


t_list* deserializar_lista_de_instrucciones (void* stream) {


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

    debug_log("serializacion.c@deserializar_lista_de_instrucciones", "El CODOP es:");
    char* log_codop_serializado = string_itoa(codop_deserializado);
    debug_log("serializacion.c@deserializar_lista_de_instrucciones", log_codop_serializado);
    debug_log("serializacion.c@deserializar_lista_de_instrucciones", "El LIST_SIZE es:");
    char* log_list_size = string_itoa(list_size_deserializado);
    debug_log("serializacion.c@deserializar_lista_de_instrucciones", log_list_size);

	for(int i=0; i< list_size_deserializado ; i++){

		instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));

		memcpy(&(instruccion_a_cargar->instruccion),stream + desplazamiento ,sizeof(uint32_t)  ) ;
        debug_log("serializacion.c@deserializar_lista_de_instrucciones","La instruccion tiene valor ENUM:");
        char* log_instruccion_enum = string_itoa(instruccion_a_cargar->instruccion);
        debug_log("serializacion.c@deserializar_lista_de_instrucciones", log_instruccion_enum);
		desplazamiento+=sizeof(uint32_t);


		memcpy(&(instruccion_a_cargar->argumentos[0]),stream + desplazamiento ,sizeof(int32_t) ) ;
        debug_log("serializacion.c@deserializar_lista_de_instrucciones","La instruccion tiene parametro 0 :");
        char* log_primer_valor = string_itoa(instruccion_a_cargar->argumentos[0]);
        debug_log("serializacion.c@deserializar_lista_de_instrucciones", log_primer_valor);
		desplazamiento+=sizeof(int32_t);

		memcpy(&(instruccion_a_cargar->argumentos[1]),stream + desplazamiento ,sizeof(int32_t) ) ;
        debug_log("serializacion.c@deserializar_lista_de_instrucciones","La instruccion tiene parametro 1 :");
        char* log_segundo_valor = string_itoa(instruccion_a_cargar->argumentos[1]);
        debug_log("serializacion.c@deserializar_lista_de_instrucciones", log_segundo_valor);
		desplazamiento+=sizeof(int32_t);


		list_add( instrucciones_deserializadas, instruccion_a_cargar);

        free(log_instruccion_enum);
        free(log_primer_valor);
        free(log_segundo_valor);
	}

    free(log_codop_serializado);
    free(log_list_size);

	return instrucciones_deserializadas;

}

void printear_instruccion(instruccion_t* una_instruccion){
    format_debug_log("serializacion.c@printear_lista", "Instruccion(numero): %d", una_instruccion->instruccion);
    format_debug_log("serializacion.c@printear_lista", "argumento 1: %d",   una_instruccion->argumentos[0]);
    format_debug_log("serializacion.c@printear_lista", "argumento 2: %d",  una_instruccion->argumentos[1]);
}
