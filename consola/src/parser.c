#include "../include/parser.h"

void cargar_argumento(instruccion_t* instruccion, char** keyAndValue){

	if(strcmp(keyAndValue[0],"NO_OP")== 0){
		instruccion->instruccion = NO_OP;
		instruccion->argumentos[0] = -1;
		instruccion->argumentos[1] = -1;

	}else if (strcmp(keyAndValue[0],"I/O")== 0){
		instruccion->instruccion = IO;
		instruccion->argumentos[0] = atoi(keyAndValue[1]);
		instruccion->argumentos[1] = -1;

	}else if (strcmp(keyAndValue[0],"READ")== 0){
		instruccion->instruccion = READ;
		instruccion->argumentos[0] = atoi(keyAndValue[1]);
		instruccion->argumentos[1] = -1;

	}else if (strcmp(keyAndValue[0],"WRITE")== 0){
		instruccion->instruccion = WRITE;
		instruccion->argumentos[0] = atoi(keyAndValue[1]);
		instruccion->argumentos[1] = atoi(keyAndValue[2]);

	}else if (strcmp(keyAndValue[0],"COPY")== 0){
		instruccion->instruccion = COPY;
		instruccion->argumentos[0] = atoi(keyAndValue[1]);
		instruccion->argumentos[1] = atoi(keyAndValue[2]);

	}else if (strcmp(keyAndValue[0],"EXIT")== 0){
		instruccion->instruccion = EXIT;
		instruccion->argumentos[0] = -1;
		instruccion->argumentos[1] = -1;

	}else{

		error_log("parser.c@cargar_argumento","Instruccion no conocida. Abortando");
        error_log("parser.c@cargar_argumento",keyAndValue[0]);
		abort();

	}

}


t_list *config_create_parser(char *path) {
	FILE* file = fopen(path, "r");

	if (file == NULL) {
        error_log("parser.c@config_create_parser", "fopen returned null.");
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);


	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	if (strstr(buffer, "\r\n")) {
		warning_log("parser.c@config_create_parser","\n\nconfig_create - WARNING: the file %s contains a \\r\\n sequence "
		 "- the Windows new line sequence. The \\r characters will remain as part "
		 "of the value, as Unix newlines consist of a single \\n. You can install "
		 "and use `dos2unix` program to convert your files to Unix style.\n\n");
        warning_log("parser.c@config_create_parser", path);
	}

	char** lines = string_split(buffer, "\n");

	t_list* lista_instrucciones = list_create();

	void add_cofiguration(char *line) {
		if (!string_is_empty(line) && !string_starts_with(line, "#")) {
            int cantidad_split = 3;
            if(string_contains(line, "EXIT")){
                cantidad_split = 1;
            }
			char** keyAndValue = string_n_split(line, cantidad_split, " ");

			if(strcmp(keyAndValue[0], "NO_OP") == 0){
                // Caso especial NO_OP. El valor de su parametro lo tomamos como N instrucciones diferentes.
				for(int i = 0; i < atoi(keyAndValue[1]); i++) {

					instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));
					cargar_argumento(instruccion_a_cargar, keyAndValue);
					list_add( lista_instrucciones, instruccion_a_cargar);

				}

			}else{

				instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));
				cargar_argumento(instruccion_a_cargar, keyAndValue);
				list_add( lista_instrucciones, instruccion_a_cargar);

			}
            debug_log("parser.c@config_create_parser",keyAndValue[0] );


			free(keyAndValue[0]);

            if(string_contains(line, "EXIT")){
                // Caso de salida especial cuando es la instrucción exit. Soluciona invalid read of size 8
                // Si es EXIT no intentamos liberar más espacios de memoria.
                free(keyAndValue);
                return;
            }

            // Es posible que la instrucción no tenga parametro (que sea null),
            // estaríamos haciendo una invalid read al liberarlo
            if(keyAndValue[1] != NULL) {
               // info_log("parser.c@config_create_parser",keyAndValue[1] );
                free(keyAndValue[1]);
            }
            if(keyAndValue[2] != NULL){
              //  info_log("parser.c@config_create_parser",keyAndValue[2] );
                free(keyAndValue[2]);
            }

			free(keyAndValue);
		}
	}
	string_iterate_lines(lines, add_cofiguration);

	string_array_destroy(lines);
	free(buffer);
	fclose(file);

	return lista_instrucciones;
}

int argumentos_por_instruccion(char* instruccion) {
    if(strcmp("NO_OP", instruccion) == 0) {
        return 1;
    }
    else if(strcmp("I/O", instruccion) == 0) {
        return 1;
    }
    else if(strcmp("READ", instruccion) == 0) {
        return 1;
    }
    else if(strcmp("WRITE", instruccion) == 0) {
        return 2;
    }
    else if(strcmp("COPY", instruccion) == 0) {
        return 2;
    }
    else if(strcmp("EXIT", instruccion) == 0) {
        return 0;
    }

    error_log("parser.c@argumentos_por_instruccion", "Instruccion desconocida");
    abort();
    return -1;
}



