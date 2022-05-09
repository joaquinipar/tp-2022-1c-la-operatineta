#include "../include/utils/serializacion_instrucciones.h"

ssize_t tamanio_stream_lista_instrucciones(t_list* lista_de_instrucciones){

	uint32_t cantidad_lista_instrucciones = (uint32_t) list_size(lista_de_instrucciones);
	uint32_t tamanio_instruccion = sizeof(uint32_t) + sizeof(int32_t) + sizeof(int32_t);
	ssize_t tamanio_stream = cantidad_lista_instrucciones + (cantidad_lista_instrucciones * tamanio_instruccion);
	return tamanio_stream; 
}



/**
 * @brief Serializa una lista de instrucciones en un *stream
 * 
 * @param lista_de_instrucciones 
 * @param desplazamiento indica a partir de qué posición del stream se va a copiar la lista de instrucciones
 * @param stream void* donde va a ir copiada la lista
 * @return ssize_t cantidad de bytes escritos en el stream
 */
ssize_t serializar_lista_de_instrucciones (t_list* lista_de_instrucciones , ssize_t desplazamiento_inicial, void *stream,ssize_t tamanio_stream) {

	//CODOP + LIST_SIZE + (UINT32 + INT32 + INT32)*

	int desplazamiento = desplazamiento_inicial;
    uint32_t cantidad_lista_instrucciones = (uint32_t) list_size(lista_de_instrucciones);

	memcpy(stream + desplazamiento, &cantidad_lista_instrucciones , sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);

	void cargar_instruccion_a_stream(instruccion_t* una_instruccion) {

		memcpy(stream + desplazamiento,&(una_instruccion->instruccion),sizeof(uint32_t) );
		desplazamiento+= sizeof(uint32_t);

		memcpy(stream + desplazamiento,&(una_instruccion->argumentos[0]),sizeof(int32_t) );
		desplazamiento+= sizeof(int32_t);

		memcpy(stream + desplazamiento,&(una_instruccion->argumentos[1]),sizeof(int32_t) );
		desplazamiento+= sizeof(int32_t);

	};

	list_iterate(lista_de_instrucciones, (void*)cargar_instruccion_a_stream);

	return tamanio_stream;
}

/**
 * @brief Dado un socket, devuelve una lista de instruccion_t
 * 
 * @param socket el socket por el que se va a recibir la lista
 * @return t_list* 
 */
t_list* recibir_lista_de_instrucciones (int socket) {
	t_list* instrucciones_deserializadas = list_create();
	uint32_t list_size_deserializado;

	recv(socket, &list_size_deserializado, sizeof(uint32_t), 0);

    format_debug_log("serializacion.c@deserializar_lista_de_instrucciones", "Hay %d instrucciones en la lista", list_size_deserializado);

	for(int i=0; i< list_size_deserializado ; i++){

		instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));

		recv(socket, &(instruccion_a_cargar->instruccion), sizeof(uint32_t), 0);
        format_debug_log("serializacion.c@deserializar_lista_de_instrucciones", "La instruccion tiene un valor ENUM: %d", instruccion_a_cargar->instruccion);

		recv(socket, &(instruccion_a_cargar->argumentos[0]), sizeof(int32_t), 0);
		format_debug_log("serializacion.c@deserializar_lista_de_instrucciones", "La instruccion tiene un parametro 0 : %d", instruccion_a_cargar->argumentos[0]);

		recv(socket, &(instruccion_a_cargar->argumentos[1]), sizeof(int32_t), 0);
		format_debug_log("serializacion.c@deserializar_lista_de_instrucciones", "La instruccion tiene un parametro 1 : %d", instruccion_a_cargar->argumentos[1]);

		list_add(instrucciones_deserializadas, instruccion_a_cargar);
	}

	return instrucciones_deserializadas;
}

void printear_instruccion(instruccion_t* una_instruccion){
    char* inst_name = string_itoa(una_instruccion->instruccion);
    char* inst_first_value = string_itoa(una_instruccion->argumentos[0]);
    char* inst_second_value = string_itoa(una_instruccion->argumentos[1]);
    debug_log("serializacion.c@printear_lista", "Instruccion(numero):");
    debug_log("serializacion.c@printear_lista",  inst_first_value);
    debug_log("serializacion.c@printear_lista", "Valores:");
    debug_log("serializacion.c@printear_lista", inst_second_value);
    free(inst_name);
    free(inst_first_value);
    free(inst_second_value);
};


ssize_t serializar_proceso(pcb_t* proceso, void** stream){

	debug_log("serializacion_instrucciones.c@serializar_proceso", "Serializando un proceso");
	//CODOP + PID + TAM + PC + TPAG + ESTADO + ESTIMACIONRAFAGA + ESTIMACION + DURACIONULTIMARAFAGA + RAFAGAACTUAL + TAM_STREAM_liST_INST
	ssize_t tam_stream_list_instrucciones = tamanio_stream_lista_instrucciones(proceso->lista_instrucciones); 
	ssize_t stream_size = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(rango_tiempo_t) + tam_stream_list_instrucciones; 
	uint32_t op_code = OPCODE_EJECUTAR; 
	*stream= malloc(stream_size);

	memcpy(*stream, &op_code, sizeof(uint32_t));
	int desplazamiento = 0; 
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->pid, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->tamanio, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->program_counter, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->tabla_paginas, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->estado, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
    memcpy(*stream + desplazamiento, &proceso->estimacion_rafaga, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->estimacion, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &proceso->duracion_ultima_rafaga, sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);	
	memcpy(*stream + desplazamiento, &proceso->rafaga_actual, sizeof(uint32_t));
	desplazamiento+= sizeof(rango_tiempo_t);	
	serializar_lista_de_instrucciones(proceso->lista_instrucciones, desplazamiento, *stream, tam_stream_list_instrucciones); 

	return stream_size; 

}