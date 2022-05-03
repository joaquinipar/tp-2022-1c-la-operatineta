#include "../include/cpu_api.h"

/*pcb_t* un_pcb_actualizado iniciar_ciclo_instruction(enviar por parametro todos los datos recibidos){

    -1 crear la estructura 
 0- decode
 1- fetch
     1.1 fetch operands

     funcion()
 2- execute     
 3- check interrupt 
 4- resultado 

}*/

void ciclo_de_instruccion(){

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
    uint32_t CODOP;



	pcb_t* pcb_cpu_harcodeado = malloc(sizeof(pcb_t));
	pcb_cpu_harcodeado->pid = 100;
	pcb_cpu_harcodeado->lista_instrucciones = lista_de_instrucciones;
	pcb_cpu_harcodeado->tamanio = 1500;
	pcb_cpu_harcodeado->program_counter = 0;

	//fetch
	fetch_instruction(pcb_cpu_harcodeado);

	instruccion_t *instruccion_a_ejecutar = malloc(sizeof(instruccion_t));

	//decode
	instruccion_a_ejecutar = decode(pcb_cpu_harcodeado);


	//fetch operand

	if(is_copy_instruction(instruccion_a_ejecutar)){
		fetch_operand(instruccion_a_ejecutar);
	}
	CODOP = execute_instruction(instruccion_a_ejecutar);





	//list_destroy_and_destroy_elements(pcb_cpu_harcodeado, (void*)&free);
	free(pcb_cpu_harcodeado);
	//free(&(pcb_cpu_harcodeado->lista_instrucciones));
	//free(&(pcb_cpu_harcodeado->tamanio));
	//free(&(pcb_cpu_harcodeado->program_counter));
	//free(&(pcb_cpu_harcodeado));

}
