

#ifndef INCLUDE_SERIALIZACION_H_
#define INCLUDE_SERIALIZACION_H_
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include "../include/parser.h"
#include <utils/protocolo_comunicacion.h>


uint32_t serializar_lista_de_instrucciones(t_list*, uint32_t, void**);
t_list* deserializar_lista_de_instrucciones (void*);
void printear_instruccion(instruccion_t* una_instruccion);


#endif /* INCLUDE_SERIALIZACION_H_ */



