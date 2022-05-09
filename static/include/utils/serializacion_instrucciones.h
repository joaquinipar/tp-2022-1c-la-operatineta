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

ssize_t serializar_lista_de_instrucciones (t_list* lista_de_instrucciones , ssize_t desplazamiento_inicial, void *stream,ssize_t tamanio_stream); 
t_list* recibir_lista_de_instrucciones (int socket);
ssize_t serializar_proceso(pcb_t* proceso, void** stream); 

#endif /* INCLUDE_SERIALIZACION_H_ */
