#include "../../include/utils/serializacion.h"

ssize_t tamanio_stream_lista_instrucciones(t_list *lista_de_instrucciones)
{

	uint32_t cantidad_lista_instrucciones = (uint32_t)list_size(lista_de_instrucciones);
	uint32_t tamanio_instruccion = sizeof(uint32_t) + sizeof(int32_t) + sizeof(int32_t);
	ssize_t tamanio_stream = sizeof(uint32_t) + (cantidad_lista_instrucciones * tamanio_instruccion);
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
ssize_t serializar_lista_de_instrucciones(t_list *lista_de_instrucciones, ssize_t desplazamiento_inicial, void *stream)
{
	//CODOP + LIST_SIZE + (UINT32 + INT32 + INT32)*
	ssize_t tamanio_stream = tamanio_stream_lista_instrucciones(lista_de_instrucciones);
	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "El tamanio del stream de la lista es %d ", tamanio_stream);
	int desplazamiento = desplazamiento_inicial;
	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "El tamanio del desplazamiento es %d ", desplazamiento);
	uint32_t cantidad_lista_instrucciones = (uint32_t)list_size(lista_de_instrucciones);
	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "Cantidad de instrucciones es %d ", cantidad_lista_instrucciones);

	memcpy(stream + desplazamiento, &cantidad_lista_instrucciones, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	void cargar_instruccion_a_stream(instruccion_t * una_instruccion)
	{

		memcpy(stream + desplazamiento, &(una_instruccion->instruccion), sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);

		memcpy(stream + desplazamiento, &(una_instruccion->argumentos[0]), sizeof(int32_t));
		desplazamiento += sizeof(int32_t);

		memcpy(stream + desplazamiento, &(una_instruccion->argumentos[1]), sizeof(int32_t));
		desplazamiento += sizeof(int32_t);
	};

	list_iterate(lista_de_instrucciones, (void *)cargar_instruccion_a_stream);
	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "El tamanio del desplazamiento es %d ", desplazamiento);

	return tamanio_stream;
}

uint32_t recibir_tamanio_proceso (int socket) {
	uint32_t tamanio;
	recv(socket, &tamanio, sizeof(uint32_t), 0);
	return tamanio;
}

/**
 * @brief Dado un socket, devuelve una lista de instruccion_t
 * 
 * @param socket el socket por el que se va a recibir la lista
 * @return t_list* 
 */
t_list *recibir_lista_de_instrucciones(int socket)
{
	t_list *instrucciones_deserializadas = list_create();
	uint32_t list_size_deserializado;

	recv(socket, &list_size_deserializado, sizeof(uint32_t), 0);

	format_debug_log("serializacion.c@deserializar_lista_de_instrucciones", "Hay %d instrucciones en la lista", list_size_deserializado);

	for (int i = 0; i < list_size_deserializado; i++)
	{

		instruccion_t *instruccion_a_cargar = malloc(sizeof(instruccion_t));

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

void printear_instruccion(instruccion_t *una_instruccion)
{
	char *inst_name = string_itoa(una_instruccion->instruccion);
	char *inst_first_value = string_itoa(una_instruccion->argumentos[0]);
	char *inst_second_value = string_itoa(una_instruccion->argumentos[1]);
	debug_log("serializacion.c@printear_lista", "Instruccion(numero):");
	debug_log("serializacion.c@printear_lista", inst_first_value);
	debug_log("serializacion.c@printear_lista", "Valores:");
	debug_log("serializacion.c@printear_lista", inst_second_value);
	free(inst_name);
	free(inst_first_value);
	free(inst_second_value);
};

uint32_t serializar_proceso(pcb_t *proceso, op_code_t codigo_operacion, void **stream)
{

	uint32_t tam_stream_list_instruc = tamanio_stream_lista_instrucciones(proceso->lista_instrucciones);

	/*COD OP  +PID+ TAMANIO+PC +  TP + ESTADO+ ESTIMACIONRAFAGA+ ESTIMACION+DURACION+RAFAGAACTUAL + cant de instru + c/instruccion* 3 parametros*/
	uint32_t stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tam_stream_list_instruc;

	*stream = malloc(stream_size);

	// format_debug_log("serializacion.c@serializar_proceso", "Stream List Instrucciones %d", tam_stream_list_instruc);
	format_debug_log("serializacion.c@serializar_proceso", "Stream Size %d", stream_size);

	int desplazamiento = 0;
	memcpy(*stream, &codigo_operacion, sizeof(op_code_t));
	desplazamiento += sizeof(op_code_t);
	memcpy(*stream + desplazamiento, &(proceso->pid), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->tamanio), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->program_counter), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->tabla_paginas), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->estado), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->estimacion_rafaga), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->estimacion), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->duracion_ultima_rafaga), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(*stream + desplazamiento, &(proceso->rafaga_actual), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	serializar_lista_de_instrucciones(proceso->lista_instrucciones, desplazamiento, *stream);

	return stream_size;
}

//CODOP + PID + TAM + PC + TPAG + ESTADO + ESTIMACIONRAFAGA + ESTIMACION + DURACIONULTIMARAFAGA + RAFAGAACTUAL + TAM_STREAM_liST_INST

pcb_t *deserializar_proceso(int cliente_socket){

	debug_log("serializacion.c@deserializar_proceso", "Deserializando un proceso");

	pcb_t *proceso_recibido = malloc(sizeof(pcb_t));
	debug_log("serializacion@deserializar_proceso", "Recibiendo los datos del proceso");
	int bytes_recibidos = recv(cliente_socket, &(proceso_recibido->pid), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Proceso: %d", proceso_recibido->pid);
	format_debug_log("serializacion@deserializar_proceso", "Bytes: %d", bytes_recibidos);
	int bytes_recibidos1 = recv(cliente_socket, &(proceso_recibido->tamanio), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Tamanio: %d", proceso_recibido->tamanio);
	format_debug_log("serializacion@deserializar_proceso", "Bytes: %d", bytes_recibidos1);
	recv(cliente_socket, &(proceso_recibido->program_counter), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Program_Counter: %d", proceso_recibido->program_counter);
	recv(cliente_socket, &(proceso_recibido->tabla_paginas), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Nro Tabla de paginas: %d", proceso_recibido->tabla_paginas);
	recv(cliente_socket, &(proceso_recibido->estado), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Estado: %d", proceso_recibido->estado);
	recv(cliente_socket, &(proceso_recibido->estimacion_rafaga), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Estimacion Rafaga: %d", proceso_recibido->estimacion_rafaga);
	recv(cliente_socket, &(proceso_recibido->estimacion), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Estimacion: %d", proceso_recibido->estimacion);
	recv(cliente_socket, &(proceso_recibido->duracion_ultima_rafaga), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Duracion Ultima Rafaga: %d", proceso_recibido->duracion_ultima_rafaga);
	recv(cliente_socket, &(proceso_recibido->rafaga_actual), sizeof(uint32_t), 0);
	format_debug_log("serializacion@deserializar_proceso", "Rafaga Actual: %d", proceso_recibido->rafaga_actual);
	proceso_recibido->lista_instrucciones = recibir_lista_de_instrucciones(cliente_socket);

	return proceso_recibido;
}
