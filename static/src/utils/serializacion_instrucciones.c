#include "../include/utils/serializacion_instrucciones.h"

/**
 * @brief Serializa una lista de instrucciones en un *stream
 * 
 * @param lista_de_instrucciones 
 * @param desplazamiento indica a partir de qué posición del stream se va a copiar la lista de instrucciones
 * @param stream void* donde va a ir copiada la lista
 * @return ssize_t cantidad de bytes escritos en el stream
 */
ssize_t serializar_lista_de_instrucciones (t_list* lista_de_instrucciones , ssize_t desplazamiento_inicial, void *stream) {

	//CODOP + LIST_SIZE + (UINT32 + INT32 + INT32)*

	uint32_t cantidad_lista_instrucciones = (uint32_t) list_size(lista_de_instrucciones);
	uint32_t tamanio_instruccion = sizeof(uint32_t) + sizeof(int32_t) + sizeof(int32_t);
	ssize_t tamanio_stream = cantidad_lista_instrucciones + (cantidad_lista_instrucciones * tamanio_instruccion);

	int desplazamiento = desplazamiento_inicial;

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
