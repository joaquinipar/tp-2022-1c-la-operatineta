#include "../include/ciclo_decode_fetch.h"



void printear_instruccion(instruccion_t* una_instruccion){

    char* inst_name;// = string_itoa(una_instruccion->instruccion);
    char* inst_first_value = string_itoa(una_instruccion->argumentos[0]);
    char* inst_second_value = string_itoa(una_instruccion->argumentos[1]);

	switch(una_instruccion->instruccion){
		case 0:
			inst_name = "NO_OP";
			break;
		case 1:
			inst_name = "I/O";
			break;
		case 2:
			inst_name = "READ";
			break;
		case 3:
			inst_name = "WRITE";
			break;
		case 4:
			inst_name = "COPY";
			break;
		case 5:
			inst_name = "EXIT";
			break;
	}

    info_log("ciclo_decode_fetch.c@printear_lista", "Instruccion:");
    info_log("ciclo_decode_fetch.c@printear_lista", inst_name);
    info_log("ciclo_decode_fetch.c@printear_lista", "Valores parametro 1:");
    info_log("ciclo_decode_fetch.c@printear_lista",  inst_first_value);
    info_log("ciclo_decode_fetch.c@printear_lista", "Valores parametro 2:");
    info_log("ciclo_decode_fetch.c@printear_lista",  inst_second_value);
    //free(inst_name);
    free(inst_first_value);
    free(inst_second_value);
};




// Funcion que avanza el program counter
void fetch_instruction(pcb_t* pcb){
	char* stringProgramCounter = string_itoa(pcb->program_counter);

	info_log("ciclo_decode_fetch.c@fetch_instruction",  "El Program Counter previo al fetch instruction está en:");
	info_log("ciclo_decode_fetch.c@fetch_instruction",  stringProgramCounter);


	pcb->program_counter++;

	stringProgramCounter = string_itoa(pcb->program_counter);
	info_log("ciclo_decode_fetch.c@fetch_instruction",  "El Program Counter luego del fetch instruction está en:");
	info_log("ciclo_decode_fetch.c@fetch_instruction",  stringProgramCounter);

	free(stringProgramCounter);

}




//decode retorna la instruccion del pbc que se debe ejecutar
instruccion_t* decode(pcb_t* pcb){

	instruccion_t* instruccion_a_analizar = malloc(sizeof(instruccion_t));

	info_log("ciclo_decode_fetch.c@decode",  "Se realiza el decode sobre la lista de instrucciones con el Program Counter");

	//Se resta en uno el contador porque se sumó en uno en la etapa de fetch
	instruccion_a_analizar = list_get(pcb->lista_instrucciones,pcb->program_counter - 1);

	printear_instruccion(instruccion_a_analizar);

	return instruccion_a_analizar;


}


void fetch_operand(instruccion_t* instruccion_a_analizar){
	char* stringDireccion_logica_destino = string_itoa(instruccion_a_analizar->argumentos[0]);
	char* stringdireccion_logica_origen = string_itoa(instruccion_a_analizar->argumentos[1]);

	//Tira warning por no utilizarlos, se dejan comentados
	//int32_t direccion_logica_destino = instruccion_a_analizar->argumentos[0];
	//int32_t direccion_logica_origen = instruccion_a_analizar->argumentos[1];

	//TODO
	//request_a_memoria_copy();

	info_log("ciclo_decode_fetch.c@fetch_operand",  "Se ha realizado una copia en memoria desde la posicion:");
	info_log("ciclo_decode_fetch.c@fetch_operand",  stringdireccion_logica_origen);


	info_log("ciclo_decode_fetch.c@fetch_operand",  "Hacia la posicion:");
	info_log("ciclo_decode_fetch.c@fetch_operand",  stringDireccion_logica_destino);


	free(stringdireccion_logica_origen);
	free(stringDireccion_logica_destino);


}


//retorna 1 si la instrucción es COPY (o 4 según valor ENUM)
int is_copy_instruction(instruccion_t* unaInstruccion){
	info_log("ciclo_decode_fetch.c@is_copy_instruction",  "La instruccion que se va a ejecutar es COPY?:");


	if(unaInstruccion->instruccion == 4){
		info_log("ciclo_decode_fetch.c@is_copy_instruction", "Si" );

		return 1;

	}else{
		info_log("ciclo_decode_fetch.c@is_copy_instruction", "No" );

		return 0;

	}

}




/*
case EJECUTAR:{
   1-Aca se debe recibir el pcb que envia Kernel.
   recv(cliente_socket, &pid, sizeof(uint32_t), false) (Asi por cada dato que se envia);

    typedef struct {
uint32_t pid;
t_list*  lista_instrucciones;
uint32_t tamanio;
uint32_t program_counter;
 }pcb_t;
   2- pcb_t* un_pcb_actualizado = iniciar_ciclo_instruction(enviar por parametro todos los datos recibidos);

   3- enviar el pcb actualizado



  return true;
  break;
}
*/
