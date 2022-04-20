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

		printf("instruccion no conocida");
		abort();

	}

}


t_list *config_create_parser(char *path) {
	FILE* file = fopen(path, "r");

	if (file == NULL) {
		return NULL;
	}

	struct stat stat_file;
	stat(path, &stat_file);


	char* buffer = calloc(1, stat_file.st_size + 1);
	fread(buffer, stat_file.st_size, 1, file);

	if (strstr(buffer, "\r\n")) {
		printf("\n\nconfig_create - WARNING: the file %s contains a \\r\\n sequence "
		 "- the Windows new line sequence. The \\r characters will remain as part "
		 "of the value, as Unix newlines consist of a single \\n. You can install "
		 "and use `dos2unix` program to convert your files to Unix style.\n\n", path);
	}

	char** lines = string_split(buffer, "\n");

	t_list* lista_instrucciones = list_create();

	void add_cofiguration(char *line) {
		if (!string_is_empty(line) && !string_starts_with(line, "#")) {
			char** keyAndValue = string_n_split(line, 3, " ");

			printf("%s %s\n", keyAndValue[0], keyAndValue[1]);

			if(strcmp(keyAndValue[0], "NO_OP") == 0){

				for(int i = 0; i < atoi(keyAndValue[1]); i++){

				instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));
				cargar_argumento(instruccion_a_cargar, keyAndValue);
				list_add( lista_instrucciones, instruccion_a_cargar);

				}

			}else{

				instruccion_t* instruccion_a_cargar = malloc(sizeof(instruccion_t));
				cargar_argumento(instruccion_a_cargar, keyAndValue);
				list_add( lista_instrucciones, instruccion_a_cargar);

			}


			free(keyAndValue[0]);
			free(keyAndValue[1]);
			free(keyAndValue[2]);
			free(keyAndValue);
		}
	}
	string_iterate_lines(lines, add_cofiguration);

	string_array_destroy(lines);
	free(buffer);
	fclose(file);

	return lista_instrucciones;
}


