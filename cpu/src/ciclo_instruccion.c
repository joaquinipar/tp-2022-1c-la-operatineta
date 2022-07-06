#include "../include/ciclo_instruccion.h"
int interrupt_flag = 0; 

void print_instruccion(instruccion_t* una_instruccion){

    char* inst_name;


	switch(una_instruccion->instruccion){
		case 0:
			inst_name = "NO_OP";
			//format_info_log("-", "Instruccion: %s", inst_name );
			//format_info_log("-", "Retardo: %d", una_instruccion->argumentos[0]);
			break;
		case 1:
			inst_name = "I/O";
			//format_info_log("-", "Instruccion: %s", inst_name );
			//format_info_log("-", "Tiempo de bloqueo: %d", una_instruccion->argumentos[0]);
			break;
		case 2:
			inst_name = "READ";
			//format_info_log("-", "Instruccion: %s", inst_name );
		//	format_info_log("-", "Direccion Logica: %d", una_instruccion->argumentos[0]);
			
			break;
		case 3:
			inst_name = "WRITE";
			//format_info_log("-", "Instruccion: %s", inst_name );
			//format_info_log("-", "Direccion Logica: %d", una_instruccion->argumentos[0]);
			//format_info_log("-", "Valor a escribir: %d", una_instruccion->argumentos[1]);
			break;
		case 4:
			inst_name = "COPY";
			//format_info_log("-", "Instruccion: %s", inst_name );
			//format_info_log("-", "Direccion Logica Destino: %d", una_instruccion->argumentos[0]);
			//format_info_log("-", "Direccion Logica Origen: %d", una_instruccion->argumentos[1]);
			break;
		case 5:
			inst_name = "EXIT";
			//format_info_log("-", "Instruccion: %s", inst_name );
			break;
	}

	format_debug_log("ciclo_instruccion.c@printear_lista", "Instruccion: %s", inst_name );
	format_debug_log("ciclo_instruccion.c@printear_lista", "Valores parametro 1: %d", una_instruccion->argumentos[0]);
    format_debug_log("ciclo_instruccion.c@printear_lista", "Valores parametro 2: %d",una_instruccion->argumentos[1]);

}
 /*
void aumentar_program_counter(pcb_t* proceso){

	format_info_log("ciclo_decode_fetch.c@fetch_instruction",  "El Program Counter previo al fetch instruction está en: %d", proceso->program_counter);

	proceso->program_counter++;

	format_info_log("ciclo_decode_fetch.c@fetch_instruction",  "El Program Counter luego del fetch instruction está en: %d", proceso->program_counter);


}*/


void encender_flag_interrupt(){
    interrupt_flag = 1;
}

bool chequear_interrupt(){
    return interrupt_flag == 1;
}

void apagar_flag_interrupt(){
    interrupt_flag = 0;
}

instruccion_t* fetch_instruction(pcb_t* proceso){

	//instruccion_t* instruccion_a_analizar = malloc(sizeof(instruccion_t));

	format_info_log("ciclo_instruccion.c@fetch_instruction",  "FETCH - Pid: %d - Program Counter: %d", proceso->pid, proceso->program_counter);

	instruccion_t* instruccion_a_analizar = list_get(proceso->lista_instrucciones, proceso->program_counter);

	print_instruccion(instruccion_a_analizar);

	return instruccion_a_analizar;

}

int32_t gestionar_instruccion_read(pcb_t* proceso, int32_t direccion_logica){

	uint32_t num_pagina = obtener_pagina(direccion_logica); 
	uint32_t entrada_1er_nivel = obtener_entrada_tabla_1er_nivel(num_pagina); 
	uint32_t entrada_2do_nivel = obtener_entrada_tabla_2do_nivel(num_pagina); 
	uint32_t dezplazamiento = obtener_desplazamiento(direccion_logica, num_pagina); 

	format_debug_log("ciclo_instruccion.c@gestionar_instruccion_read", "Numero de Pagina: %d", num_pagina);
	format_debug_log("ciclo_instruccion.c@gestionar_instruccion_read", "Entrada 1er Nivel: %d", entrada_1er_nivel);
	format_debug_log("ciclo_instruccion.c@gestionar_instruccion_read", "Entrada 2do Nivel: %d", entrada_2do_nivel);
	format_debug_log("ciclo_instruccion.c@gestionar_instruccion_read", "Dezplazamiento: %d", dezplazamiento);

	uint32_t valor_a_imprimir;

	//Chequeo si num de pagina se encuentra en la tlb. -1 no se encuentra sino devuelve el marco asignado al num de pagina
	uint32_t marco_asignado = se_encuentra_en_tlb(num_pagina); 

	if(marco_asignado != -1){
		uint32_t direccion_fisica = obtener_direccion_fisica(marco_asignado, dezplazamiento); 
		format_info_log("ciclo_instruccion.c@gestionar_instruccion_read", "Direccion Fisica: %d", direccion_fisica);
		valor_a_imprimir = enviar_mensaje_read(proceso->pid, direccion_fisica); 
		format_info_log("ciclo_instruccion.c@gestionar_instruccion_read", "Valor obtenido de Memoria: %i", valor_a_imprimir);
		return valor_a_imprimir; 
	}

	//CASO QUE EL MARCO DE LA PAGINA NO SE ENCUENTRA EN LA TLB

	//1- Enviar mensaje a memoria para que me informe el numero de la tabla de 2do nivel correspondiente. 
	uint32_t num_tabla_2do_nivel = enviar_mensaje_acceso_1er_nivel(proceso->pid, entrada_1er_nivel, proceso->tabla_paginas); 

	if(num_tabla_2do_nivel != -1){
		//2- Enviar mensaje a memoria para que me informe el numero de marco correspondiente a nro tabla 2do nivel entrada 2do nivel  
		uint32_t marco_recibido = enviar_mensaje_acceso_2do_nivel(proceso->pid, num_tabla_2do_nivel, entrada_2do_nivel); 
        format_debug_log("ciclo_instruccion.c@gestionar_instruccion_read", "MARCO RECIBIDO %i", marco_recibido);
		   //2.1 Se carga marco en la tlb
			 uint32_t resultado = escribir_entrada_en_tlb(num_pagina, marco_recibido); 
				if(!resultado){
					format_error_log("ciclo_instruccion.c@gestionar_instruccion_read", "Error al escribir entrada en TLB - Pagina: %d - Marco: %d",num_pagina, marco_recibido); 
				}
		if(marco_recibido != -1){

			//3- Envio mensaje a memoria para acceder a la porcion de memoria correspiente a la DF calculada
			uint32_t dir_fisica= obtener_direccion_fisica(marco_recibido, dezplazamiento); 
			format_info_log("ciclo_instruccion.c@gestionar_instruccion_read", "Direccion Fisica: %d", dir_fisica);
			valor_a_imprimir = enviar_mensaje_read(proceso->pid, dir_fisica); 
			format_info_log("ciclo_instruccion.c@gestionar_instruccion_read", "Valor obtenido de Memoria: %i", valor_a_imprimir);
			return valor_a_imprimir; 
			
		}
	}
	format_error_log("ciclo_instruccion.c@gestionar_instruccion_read", "ERROR AL LEER EN MEMORIA");
	return -1; 
}

void gestionar_instruccion_write(pcb_t* proceso, int32_t direccion_logica, int32_t valor_a_copiar){

	uint32_t num_pagina = obtener_pagina(direccion_logica); 
	uint32_t entrada_1er_nivel = obtener_entrada_tabla_1er_nivel(num_pagina); 
	uint32_t entrada_2do_nivel = obtener_entrada_tabla_2do_nivel(num_pagina); 
	uint32_t dezplazamiento = obtener_desplazamiento(direccion_logica, num_pagina); 

	format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "PID: %d - Numero de Pagina: %d", proceso->pid, num_pagina);
	format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "PID: %d - Entrada 1er Nivel: %d", proceso->pid, entrada_1er_nivel);
	format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "PID: %d - Entrada 2do Nivel: %d", proceso->pid, entrada_2do_nivel);
	format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "PID: %d - Dezplazamiento: %d", proceso->pid, dezplazamiento);
	format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "PID: %d - Valor a copiar: %d", proceso->pid, valor_a_copiar); 


	//Chequeo si num de pagina se encuentra en la tlb. -1 no se encuentra sino devuelve el marco asignado al num de pagina
	uint32_t marco_asignado = se_encuentra_en_tlb(num_pagina); 

	if(marco_asignado != -1){
		uint32_t direccion_fisica = obtener_direccion_fisica(marco_asignado, dezplazamiento); 
		format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "Direccion Fisica: %d", direccion_fisica);
		format_debug_log("ciclo_instruccion.c@gestionar_instruccion_write", "Valor a copiar en Memoria: %d", valor_a_copiar);
		enviar_mensaje_write(proceso->pid, direccion_fisica, valor_a_copiar);
        return;
	}

	//CASO QUE EL MARCO DE LA PAGINA NO SE ENCUENTRA EN LA TLB

	//1- Enviar mensaje a memoria para que me informe el numero de la tabla de 2do nivel correspondiente. 
	uint32_t num_tabla_2do_nivel = enviar_mensaje_acceso_1er_nivel(proceso->pid, entrada_1er_nivel, proceso->tabla_paginas); 

	if(num_tabla_2do_nivel != -1){
		//2- Enviar mensaje a memoria para que me informe el numero de marco correspondiente a nro tabla 2do nivel entrada 2do nivel  
		uint32_t marco_recibido = enviar_mensaje_acceso_2do_nivel(proceso->pid, num_tabla_2do_nivel, entrada_2do_nivel); 

		   //2.1 Se carga marco en la tlb
			 uint32_t resultado = escribir_entrada_en_tlb(num_pagina, marco_recibido); 
				if(!resultado){
					format_error_log("ciclo_instruccion.c@gestionar_instruccion_write", "Error al escribir entrada en TLB - Pagina: %d - Marco: %d",num_pagina, marco_recibido); 
				}
		if(marco_recibido != -1){

			//3- Envio mensaje a memoria para acceder a la porcion de memoria correspiente a la DF calculada
			uint32_t dir_fisica= obtener_direccion_fisica(marco_recibido, dezplazamiento); 
			format_info_log("ciclo_instruccion.c@gestionar_instruccion_write", "Direccion Fisica: %d", dir_fisica);
		    format_debug_log("ciclo_instruccion.c@gestionar_instruccion_write", "Valor a copiar en Memoria: %d", valor_a_copiar);
			enviar_mensaje_write(proceso->pid, dir_fisica, valor_a_copiar); 
			return;
		}
	}

	format_error_log("ciclo_instruccion.c@gestionar_instruccion_write", "ERROR AL ESCRIBIR EN MEMORIA"); 

}


void gestionar_instruccion_copy(pcb_t* proceso, instruccion_t* instruccion){

	int32_t valor = gestionar_instruccion_read(proceso, instruccion->argumentos[1]); 
	gestionar_instruccion_write(proceso, instruccion->argumentos[0], valor); 

}

pcb_t* execute_instruction(instruccion_t* instruccion_a_ejecutar, pcb_t* proceso){

	switch(instruccion_a_ejecutar->instruccion){
		case 0: //INSTRUCCION NO_OP
			format_info_log("ciclo_instruccion.c@execute_instruction",  "NOOP - PID: %d - Se realizará un sleep de: %d ",proceso->pid, cpu_config->retardo_noop);
			proceso->program_counter++;
			usleep(cpu_config->retardo_noop * 1000);
			return proceso;
			break;
		case 1: //INSTRUCCION I/O
			debug_log("ciclo_instruccion.c@execute_instruction",  "Se realizará I/O. Requiere desalojo");
			format_info_log("ciclo_instruccion.c@execute_instruction",  "I/O -  PID: %d - Tiempo de bloqueo: %d", proceso->pid, instruccion_a_ejecutar->argumentos[0]);
			proceso->program_counter++;
			enviar_mensaje_proceso_desalojado_io(proceso, cliente_socket, instruccion_a_ejecutar->argumentos[0]); 
			eliminar_entradas_TLB(); 
			return proceso;
			break;
		case 2: //INSTRUCCION READ
			format_info_log("ciclo_instruccion.c@execute_instruction",  "READ - PID: %d - Direccion Logica:%d ",proceso->pid, instruccion_a_ejecutar->argumentos[0]);
			gestionar_instruccion_read(proceso, instruccion_a_ejecutar->argumentos[0]); 
			proceso->program_counter++;
            imprimir_estado_array_TLB();
			return proceso;
			break;
		case 3://INSTRUCCION WRITE
			format_info_log("ciclo_instruccion.c@execute_instruction",  "WRITE - PID: %d - Direccion Logica:%d - Valor a escribir: %d ",proceso->pid, instruccion_a_ejecutar->argumentos[0], instruccion_a_ejecutar->argumentos[1]);
			gestionar_instruccion_write(proceso, instruccion_a_ejecutar->argumentos[0], instruccion_a_ejecutar->argumentos[1]); 
			proceso->program_counter++;
            imprimir_estado_array_TLB();
			return proceso;
			break;
		case 4://INSTRUCCION COPY
			format_info_log("ciclo_instruccion.c@execute_instruction",  "COPY - PID: %d - Direccion Logica Destino :%d - Direccion Logica Origen / DL: %d ",proceso->pid, instruccion_a_ejecutar->argumentos[0], instruccion_a_ejecutar->argumentos[1]);			
			gestionar_instruccion_copy(proceso, instruccion_a_ejecutar); 
			proceso->program_counter++;
            imprimir_estado_array_TLB();
			return proceso;
			break;
		case 5://INSTRUCCION EXIT
			debug_log("ciclo_instruccion.c@execute_instruction",  "Se realizará un EXIT. Requiere desalojo");
			format_info_log("ciclo_instruccion.c@execute_instruction",  "EXIT - PID: %d",proceso->pid);
			proceso->program_counter++;
			enviar_mensaje_proceso_desalojado_exit(proceso, cliente_socket); 
			eliminar_entradas_TLB(); 
			return proceso;
			break;
		}


	format_error_log("ciclo_instruccion.c@execute_instruccion", "Error al procesar una instruccion - PID: %d", proceso->pid); 
	return proceso;
}


bool requiere_desalojo(instruccion_t* instruccion){

  if(instruccion->instruccion == IO || instruccion->instruccion == EXIT){
	  format_info_log("ciclo_instruccion.c@requiere_desalojo", "Requiere desalojo - Instruccion: %d", instruccion->instruccion); 
	  return true;
  }
  format_info_log("ciclo_instruccion.c@requiere_desalojo", "NO - Requiere desalojo - Instruccion: %d", instruccion->instruccion);
  return false; 

}

bool hay_interrupcion(pcb_t * proceso){

	if(chequear_interrupt()){
		info_log("ciclo_instruccion.c@hay_interrupcion",  "¿Hay interrupcion?: Si");
		enviar_mensaje_proceso_desalojado_interrupt(proceso, cliente_socket); 
		apagar_flag_interrupt(); 
		eliminar_entradas_TLB(); 
		return true;
	}
	
	info_log("ciclo_instruccion.c@hay_interrupcion",  "¿Hay interrupcion?: No");
	return false;

}

 void iniciar_ciclo_instruccion(pcb_t* proceso){

   //TODO Consultar si puede haber una interrupcion en este momento antes de ejecutar la 1era instruccion
   //TODO Podria pasar que haya desalojo (i/o - exit ) e interrupcion al mismo tiempo.

   // Seteo pid en la TLB
   for(int i=0; i < cpu_config->entradas_tlb; i++){
       array_tlb[i].id_proceso = proceso->pid;
   }

   instruccion_t* instruccion_a_ejecutar; 

   do{
		format_debug_log("ciclo_instruccion.c@iniciar_ciclo_instruccion", "procesando instruccion: %d, para el proceso: %d ", proceso->program_counter, proceso->pid );
		
		instruccion_a_ejecutar = fetch_instruction(proceso); 
   		proceso = execute_instruction(instruccion_a_ejecutar, proceso); 
		
   }while(!requiere_desalojo(instruccion_a_ejecutar) && !hay_interrupcion(proceso));

 }