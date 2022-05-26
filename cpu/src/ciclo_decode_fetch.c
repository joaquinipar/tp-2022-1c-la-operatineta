#include "../include/ciclo_decode_fetch.h"



void printear_instruccion(instruccion_t* una_instruccion){

    char* inst_name;


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

	format_info_log("ciclo_decode_fetch.c@printear_lista", "Instruccion: %s", inst_name );
	format_info_log("ciclo_decode_fetch.c@printear_lista", "Valores parametro 1: %d", una_instruccion->argumentos[0]);
    format_info_log("ciclo_decode_fetch.c@printear_lista", "Valores parametro 2: %d",una_instruccion->argumentos[1]);

};




// Funcion que avanza el program counter
void aumentar_program_counter(pcb_t* pcb){

	format_info_log("ciclo_decode_fetch.c@fetch_instruction",  "El Program Counter previo al fetch instruction está en: %d", pcb->program_counter);

	pcb->program_counter++;

	format_info_log("ciclo_decode_fetch.c@fetch_instruction",  "El Program Counter luego del fetch instruction está en: %d", pcb->program_counter);


}




//decode retorna la instruccion del pbc que se debe ejecutar
instruccion_t* fetch_instruction(pcb_t* pcb){

	instruccion_t* instruccion_a_analizar = malloc(sizeof(instruccion_t));

	info_log("ciclo_decode_fetch.c@decode",  "Se realiza el decode sobre la lista de instrucciones con el Program Counter");

	//Se resta en uno el contador porque se sumó en uno en la etapa de fetch
	instruccion_a_analizar = list_get(pcb->lista_instrucciones,pcb->program_counter);

	printear_instruccion(instruccion_a_analizar);

	return instruccion_a_analizar;


}


void fetch_operand(instruccion_t* instruccion_a_analizar){

	//Tira warning por no utilizarlos, se dejan comentados
	//int32_t direccion_logica_destino = instruccion_a_analizar->argumentos[0];
	//int32_t direccion_logica_origen = instruccion_a_analizar->argumentos[1];

	//TODO
	//request_a_memoria_copy();
	format_info_log("ciclo_decode_fetch.c@fetch_operand", "Se ha realizado una copia en memoria desde la posicion: %d", instruccion_a_analizar->argumentos[1]);
	format_info_log("ciclo_decode_fetch.c@fetch_operand", "Hacia la posicion: %d",instruccion_a_analizar->argumentos[0]);





}


//retorna 1 si la instrucción es COPY (o 4 según valor ENUM)
int is_copy_instruction(instruccion_t* instruccion_a_analizar){
	info_log("ciclo_decode_fetch.c@is_copy_instruction",  "La instruccion que se va a ejecutar es COPY?:");

	if(instruccion_a_analizar->instruccion == 4){
		info_log("ciclo_decode_fetch.c@is_copy_instruction", "Si" );

		return 1;

	}else{
		info_log("ciclo_decode_fetch.c@is_copy_instruction", "No" );

		return 0;

	}

}







uint32_t execute_instruction(instruccion_t* instruccion_a_ejecutar){
	op_code_t  CODOP;
	switch(instruccion_a_ejecutar->instruccion){
			case 0: //INSTRUCCION NO_OP
				format_info_log("ciclo_decode_fetch.c@execute_instruction",  "Se realizará un sleep de: %d",cpu_config->retardo_noop);

				usleep(cpu_config->retardo_noop);
				CODOP = 1000; //OPCODE_
				return CODOP;
				break;



			case 1: //INSTRUCCION I/O
				CODOP = OPCODE_PROCESO_DESALOJADO_IO; //OPCODE_CPU_A_KERNEL_IO
				setear_variable_de_IO(instruccion_a_ejecutar->argumentos[0]);
				return CODOP;
				break;
			case 2: //INSTRUCCION READ
				CODOP = 0;
				return CODOP;
				break;
			case 3://INSTRUCCION WRITE
				CODOP = 0;
				return CODOP;
				break;
			case 4://INSTRUCCION COPY
				CODOP = 0;
				return CODOP;
				break;
			case 5://INSTRUCCION EXIT
				CODOP = OPCODE_PROCESO_DESALOJADO_EXIT;
				return CODOP;
				break;
		}

	return 0;
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
