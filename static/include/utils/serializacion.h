#ifndef INCLUDE_SERIALIZACION_H_
#define INCLUDE_SERIALIZACION_H_
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <utils/protocolo_comunicacion.h>
#include <utils/structs.h>
#include <utils/logger.h>

ssize_t tamanio_stream_lista_instrucciones(t_list* lista_de_instrucciones); 
ssize_t serializar_lista_de_instrucciones (t_list* lista_de_instrucciones , ssize_t desplazamiento_inicial, void *stream); 
t_list* recibir_lista_de_instrucciones (int socket);
uint32_t serializar_proceso(pcb_t* proceso, op_code_t codigo_operacion, void** stream); 
pcb_t* deserializar_proceso(int cliente_socket); 

#endif /* INCLUDE_SERIALIZACION_H_ */
