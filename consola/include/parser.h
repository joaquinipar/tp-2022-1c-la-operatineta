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
#include <utils/structs.h>

t_list *config_create_parser(char *path);
void cargar_argumento(instruccion_t* instruccion, char** keyAndValue);

#endif /* INCLUDE_PARSER_H_ */
