#include "../include/cpu_api.h"




void procesar_solicitud_de_kernel(pcb_t* unPcbAReemplazar){

    op_code_t CODOP = 0;

    while( (!hay_interrupcion() ) || (CODOP == 0 )){
        CODOP = ciclo_de_instruccion(unPcbAReemplazar);
    }

    if (hay_interrupcion() == 1){
    	CODOP = 10000; //OPCODE_CPU_A_KERNEL_INTERRUPCION
    }


}

op_code_t ciclo_de_instruccion(pcb_t* unPcbAReemplazar){

    t_list* lista_de_instrucciones = list_create();
    instruccion_t instruccion1;instruccion1.instruccion = 0;instruccion1.argumentos[0] =  -1;instruccion1.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion1);
    instruccion_t instruccion2;instruccion2.instruccion = 1;instruccion2.argumentos[0] = 100;instruccion2.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion2);
    instruccion_t instruccion3;instruccion3.instruccion = 2;instruccion3.argumentos[0] = 500;instruccion3.argumentos[1] = -1;
    list_add(lista_de_instrucciones, &instruccion3);
    instruccion_t instruccion4;instruccion4.instruccion = 3;instruccion4.argumentos[0] = 199;instruccion4.argumentos[1] = 400;
    list_add(lista_de_instrucciones, &instruccion4);
    instruccion_t instruccion5;instruccion5.instruccion = 4;instruccion5.argumentos[0] = 3200;instruccion5.argumentos[1] = 6500;
    list_add(lista_de_instrucciones, &instruccion5);
    op_code_t CODOP;



	pcb_t* pcb_cpu_harcodeado = malloc(sizeof(pcb_t));
	pcb_cpu_harcodeado->pid = 100;
	pcb_cpu_harcodeado->lista_instrucciones = lista_de_instrucciones;
	pcb_cpu_harcodeado->tamanio = 1500;
	pcb_cpu_harcodeado->program_counter = 0;

	//fetch
	fetch_instruction(pcb_cpu_harcodeado);

	//decode
	instruccion_t *instruccion_a_ejecutar;	instruccion_a_ejecutar = decode(pcb_cpu_harcodeado);

	//fetch operand
	if(is_copy_instruction(instruccion_a_ejecutar)){
		fetch_operand(instruccion_a_ejecutar);
	}

	//execute
	CODOP = execute_instruction(instruccion_a_ejecutar);





	free(pcb_cpu_harcodeado);
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


void setear_variable_de_interrupciones (int valor){
	INTERRUPTION_FLAG = valor;
}


void inicializar_interruption_flag(){
	INTERRUPTION_FLAG = 0;
}

