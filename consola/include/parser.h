#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#include <commons/config.h>
#include <stdlib.h>
#include <commons/collections/dictionary.h>
#include <sys/stat.h>
#include <string.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdint.h>
#include <commons/collections/list.h>
#include <utils/logger.h>

typedef enum{

	NO_OP = 0,
	IO = 1,
	READ = 2,
	WRITE = 3,
	COPY = 4,
	EXIT = 5

} instruccion_code_t;

typedef struct {

	instruccion_code_t instruccion;
	int32_t argumentos[2];

}instruccion_t;

t_list *config_create_parser(char *path);
void cargar_argumento(instruccion_t* instruccion, char** keyAndValue);

#endif /* INCLUDE_PARSER_H_ */
