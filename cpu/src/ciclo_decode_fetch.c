#include "../include/ciclo_decode_fetch.h"





// Funcion que avanza el program counter
void fetch_instruction(pcb_t* pcb){

	pcb->program_counter++;



}


//decode retorna la instruccion del pbc que se debe ejecutar
instruccion_t* decode(pcb_t* pcb){

	instruccion_t* instruccion_a_analizar = malloc(sizeof(instruccion_t));
	instruccion_a_analizar = list_get(pcb->lista_instrucciones,pcb->program_counter - 1);
	return instruccion_a_analizar;


	//TODO
	//Como se libera la memoria aca????
}


void fetch_operand(instruccion_t* instruccion_a_analizar){
	//TODO

	int32_t direccion_logica_destino = instruccion_a_analizar->argumentos[0];
	int32_t direccion_logica_origen = instruccion_a_analizar->argumentos[1];

	//TODO
	//request_a_memoria_copy();

	printf("Se ha realizado una copia en memoria desde %i hacia %i.\n",(int32_t)direccion_logica_destino,(int32_t)direccion_logica_origen);


}


//retorna 1 si la instrucción es COPY (o 4 según valor ENUM)
int is_copy_instruction(instruccion_t* unaInstruccion){

	if(unaInstruccion->instruccion == 4)
		return 1;
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
