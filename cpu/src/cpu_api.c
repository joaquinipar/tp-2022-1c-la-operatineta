#include "../include/cpu_api.h"




op_code_t procesar_solicitud_de_kernel(pcb_t* unPcbAReemplazar){

	////////////////BORRAR//////////////////
    t_list* lista_de_instrucciones = list_create();
    instruccion_t instruccion1;instruccion1.instruccion = 0;instruccion1.argumentos[0] =  -1;instruccion1.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion1);
    instruccion_t instruccion2;instruccion2.instruccion = 0;instruccion2.argumentos[0] = -1;instruccion2.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion2);
    instruccion_t instruccion3;instruccion3.instruccion = 0;instruccion3.argumentos[0] = -1;instruccion3.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion3);
    instruccion_t instruccion4;instruccion4.instruccion = 0;instruccion4.argumentos[0] = -1;instruccion4.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion4);
    instruccion_t instruccion5;instruccion5.instruccion = 1;instruccion5.argumentos[0] = 3200;instruccion5.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion5);
	pcb_t* pcb_cpu_harcodeado = malloc(sizeof(pcb_t));
	pcb_cpu_harcodeado->pid = 100;
	pcb_cpu_harcodeado->lista_instrucciones = lista_de_instrucciones;
	pcb_cpu_harcodeado->tamanio = 1500;
	pcb_cpu_harcodeado->program_counter = 0;

	///////////////////////////////////////

    op_code_t CODOP = 0;

    while( (!hay_interrupcion() ) && (!requiere_desalojo(CODOP) )){ //TODO CODOP DE EJECUCION DE PROCESOS
    	info_log("cpu_api.c@procesar_solicitud_de_kernel",  "SE REALIZA UN CICLO DE INSTRUCCION \n");
        CODOP = ciclo_de_instruccion(pcb_cpu_harcodeado);


    if (hay_interrupcion() == 1){
    	CODOP = OPCODE_PROCESO_DESALOJADO_INTERRUPT; //OPCODE_CPU_A_KERNEL_INTERRUPCION
    	//Si hay una interrupción, cargamos el código correspondiente para devolver el PCB a Kernel.
    	//Se setea en 0 la interrupción y se marca como "atendida"
    	setear_variable_de_interrupciones(0);
    	return CODOP; // CODOP DE RESPUESTA DE INTERRUPCION A KERNEL
    }
    }

	free(pcb_cpu_harcodeado);
    return CODOP;

}

op_code_t ciclo_de_instruccion(pcb_t* unPcbAReemplazar){


    op_code_t CODOP;

	//Fetch
	instruccion_t *instruccion_a_ejecutar;
	instruccion_a_ejecutar = fetch_instruction(unPcbAReemplazar);


	//Decode & fetch operand
	if(is_copy_instruction(instruccion_a_ejecutar)){
		fetch_operand(instruccion_a_ejecutar);
	}


	//execute
	CODOP = execute_instruction(instruccion_a_ejecutar);


	//aumentar Program Counter
	aumentar_program_counter(unPcbAReemplazar);

	return(CODOP);

}



int hay_interrupcion(){

	if(INTERRUPTION_FLAG == 1){
		info_log("ciclo_decode_fetch.c@check_interruption",  "¿Hay interrupcion?: Si");

		return 1;

	}else{
		info_log("ciclo_decode_fetch.c@check_interruption",  "¿Hay interrupcion?: No");
		return 0;

	}

}

int requiere_desalojo(op_code_t CODOP){
	switch(CODOP){
	case OPCODE_PROCESO_DESALOJADO_IO:
		info_log("requiere_desalojo.c@requiere_desalojo",  "¿Requiere desalojo?: Si");

		return 1;
		break;
	case OPCODE_PROCESO_DESALOJADO_EXIT:
		info_log("requiere_desalojo.c@requiere_desalojo",  "¿Requiere desalojo?: Si");
		return 1;
		break;

	case 0:
		info_log("requiere_desalojo.c@requiere_desalojo",  "¿Requiere desalojo?: No");
		return 0 ;
	}

	return 0;

}

void setear_variable_de_interrupciones (uint32_t valor){
	INTERRUPTION_FLAG = valor;
}

void setear_variable_de_IO (uint32_t valor){
	I_O_FLAG = valor;
}

void inicializar_variables_globales(){
	INTERRUPTION_FLAG = 0;
	I_O_FLAG = 0;
}



