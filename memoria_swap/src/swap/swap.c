#include "../../include/swap/swap.h"

void iniciar_swap() {
    info_log("swap.c@iniciar_swap", "Se inicializa estructuras de Swap"); 
    list_archivos_swap = list_create();

    //Valida la existencia del directorio. Si errno = 2 (ENOENT), crea el directorio.
    if (0 != access(mem_swap_config->path_swap, F_OK)) {
    	if (ENOENT == errno) {
            warning_log("swap.c@iniciar_swap", "No existe el directorio de swap, vamos a tratar de crearlo");
            // does not exist
    		if(mkdir(mem_swap_config->path_swap,0777) == 0 )
    			debug_log("swap.c@iniciar_swap", "Se crea directorio swap");
    		else
    			warning_log("swap.c@iniciar_swap", "No se pudo crear el directorio swap");
    	}
        errno = 0;
    }
    //debug_log("swap.c@iniciar_swap", "Finaliza el inicio de la lista global de archivos de swap por proceso"); 
}

void destruir_list_swap(){

    void _eliminar_ficheros(archivo_pid_t* proceso_swap){
    	//eliminar_archivo_directorio_swap(proceso_swap->pid);
        free(proceso_swap->path_archivo);
        free(proceso_swap->array_marcos_virtual_del_proceso);
        format_info_log("swap.c@destruir_list_swap", "ELIMINO ARCHIVO PID %d TAMANIO ", proceso_swap->pid, proceso_swap->tam_proceso);
        munmap(proceso_swap->area_archivo_swap, proceso_swap->tam_proceso);
    }
	list_iterate(list_archivos_swap,(void*) _eliminar_ficheros);
    list_destroy_and_destroy_elements(list_archivos_swap, (void*)free); 
    debug_log("swap.c@destruir_list_swap", "Se destruye la lista global de archivos de swap por proceso");
}

archivo_pid_t* crear_archivo_pid(int pid, uint32_t tam_proceso){
    archivo_pid_t* archivo = malloc(sizeof(archivo_pid_t));
    archivo->pid = pid;
    archivo->path_archivo = obtener_ruta_swap_proceso(pid);
    archivo->tam_proceso = tam_proceso;
    crear_archivo_swap(archivo);
    int cantidad_marcos = get_cantidad_marcos(tam_proceso);
    archivo->array_marcos_virtual_del_proceso = malloc(cantidad_marcos * sizeof(marco_virtual_t));

    for(int i=0; i < cantidad_marcos; i++){
        archivo->array_marcos_virtual_del_proceso[i].pagina = i;
    }

    return archivo;
}

uint32_t get_cantidad_marcos(uint32_t tam_proceso) {

    double resultado;

    resultado = tam_proceso / mem_swap_config->tam_pagina;

    if (resultado <= 1) {
        /* Caso: El tama??o es menor de una pagina, o es una pagina justa */
        return 1;
    } else if (tam_proceso % mem_swap_config->tam_pagina == 0) {
        return resultado;
    } else {
        /* Caso: El tama??o es mayor a una pagina y hay resto */
        return (uint32_t)resultado + 1;
    }
}



void admitir_proceso_en_swap(uint32_t pid, uint32_t tamanio){
    format_info_log("swap.c@admitir_proceso_en_swap", "Admision en Swap - PID: %d  -- Tama??o %d", pid, tamanio); 
    archivo_pid_t* un_arch_swap = crear_archivo_pid(pid, tamanio); 
    list_add(list_archivos_swap, un_arch_swap); 
}


int buscar_proceso_swap(uint32_t pid){

    bool _encontrar_proceso_en_lista_global_swap(archivo_pid_t* proceso_swap){
        return pid == proceso_swap->pid;
    }

    archivo_pid_t* proceso_swap = list_find(list_archivos_swap, (void*)&_encontrar_proceso_en_lista_global_swap);
    if(proceso_swap != NULL){
        return 1; 
    }
    return 0; 
}

archivo_pid_t* get_proceso_swap(uint32_t pid){
  if(buscar_proceso_swap(pid)==1){
  archivo_pid_t *proceso_buscado = NULL;
  bool pid_iguales(archivo_pid_t * proceso) { return proceso->pid == pid; }
  proceso_buscado = list_find(list_archivos_swap, (void *)pid_iguales);
  return proceso_buscado; 
  }
  return NULL;
}


int eliminar_proceso_en_lista_global_swap(uint32_t pid) {

  bool is_pid(archivo_pid_t * proceso_aux) { 
    return proceso_aux->pid == pid; 
    }
  archivo_pid_t *proceso_a_eliminar =list_remove_by_condition(list_archivos_swap, (void *)is_pid);

  eliminar_archivo_directorio_swap(pid);

  if (proceso_a_eliminar == NULL) {
      error_log("swap.c@eliminar_proceso_en_lista_global_swap", "[ERROR] - No se pudo eliminar proceso de la lista global de swap"); 
    return 0;
  } 

    format_debug_log("swap.c@eliminar_proceso_en_lista_global_swap", "Se elimino el proceso %d de la lista global de swap", pid);
    return 1;
}

int escribir_pagina_swap(uint32_t pid, uint32_t pagina){

    archivo_pid_t* archivo = get_proceso_swap(pid);

    if(archivo == NULL){
        format_error_log("swap.c@escribir_pagina_swap", "El proceso %i no tiene archivo swap creado.", pid);
        return -1;
    }
    info_log("swap.c@escribir_pagina_swap", "Ejecutando retardo swap...");
    usleep(mem_swap_config->retardo_swap * 1000);

    uint32_t marco = obtener_marco_de_pagina_proceso(pid, pagina, archivo); 

    memcpy(archivo->area_archivo_swap + marco * mem_swap_config->tam_pagina, mem_ppal->memoria_principal + marco * mem_swap_config->tam_pagina, mem_swap_config->tam_pagina);

    format_info_log("swap.c@escribir_pagina_swap", "PID: %i - Pagina: %d - Marco: %i escrito en SWAP correctamente! DIR: %i",pid,pagina, marco, marco * mem_swap_config->tam_pagina);


    if( msync( archivo->area_archivo_swap, archivo->tam_proceso , MS_SYNC) < 0){ // ??Est?? bien ese tama??o no?
            format_error_log("swap.c@escribir_pagina_swap", "ERROR al escribir cambios en SWAP - Proceso: %d", pid);

    }


    return 1;
}

int escribir_contenido_en_swap(uint32_t pid, uint32_t pagina, void* contenido_marco){

    archivo_pid_t* archivo = get_proceso_swap(pid);

    if(archivo == NULL){
        format_error_log("swap.c@escribir_pagina_swap", "El proceso %i no tiene archivo swap creado.", pid);
        return -1;
    }
    info_log("swap.c@escribir_pagina_swap", "Ejecutando retardo swap...");
    usleep(mem_swap_config->retardo_swap * 1000);

    uint32_t marco = obtener_marco_de_pagina_proceso(pid, pagina, archivo); 

    memcpy(archivo->area_archivo_swap + marco * mem_swap_config->tam_pagina, contenido_marco , mem_swap_config->tam_pagina);

    format_info_log("swap.c@escribir_pagina_swap", "PID: %i - Pagina: %d - Marco: %i escrito en SWAP correctamente! DIR: %i",pid,pagina, marco, marco * mem_swap_config->tam_pagina);


    if( msync( archivo->area_archivo_swap, archivo->tam_proceso , MS_SYNC) < 0){ // ??Est?? bien ese tama??o no?
            format_error_log("swap.c@escribir_pagina_swap", "ERROR al escribir cambios en SWAP - Proceso: %d", pid);

    }


    return 1;


}


int obtener_marco_de_pagina_proceso (uint32_t proceso_id, uint32_t pagina, archivo_pid_t* archivo){

	int cantidad_marcos =  archivo->tam_proceso/ mem_swap_config->tam_pagina;

    format_debug_log("swap.c@obtener_marco_de_pagina_proceso","Busco el marco que contiene al proceso %i, pagina %i",proceso_id, pagina);


	if(archivo == NULL){
		format_error_log("swap.c@obtener_marco_de_pagina_proceso","No se encontro el archivo del proceso: %d. No se puede leer", proceso_id);
		return -1;
	}

    int marco_asignado = -1;
	for(int marco = 0; marco < cantidad_marcos; marco++){

		if( archivo->array_marcos_virtual_del_proceso[marco].pagina == pagina){
            //encontro marco que tiene la pagina del proceso a leer
            marco_asignado = marco;
            break; 
		}
	}
    format_debug_log("swap.c@obtener_marco_de_pagina_proceso", "El marco que contiene a la pagina %d del proceso %i es: %i", pagina, proceso_id, marco_asignado); 
    return marco_asignado; 
}

void* leer_pagina_swap(uint32_t pid, uint32_t pagina){

    archivo_pid_t* archivo = get_proceso_swap(pid);

    if(archivo == NULL){
        format_error_log("swap.c@leer_pagina_swap", "El proceso %i no tiene archivo swap creado.", pid);
        return NULL;
    }

    usleep(mem_swap_config->retardo_swap * 1000);
    format_info_log("swap.c@leer_pagina_swap", "Se realiza retardo por acceso a Swap - PID: %d", pid); 

    uint32_t marco = obtener_marco_de_pagina_proceso(pid, pagina, archivo); 

    void* contenido = malloc(sizeof(char) * mem_swap_config->tam_pagina);

    if(marco != -1){
        memcpy(contenido, archivo->area_archivo_swap + marco * mem_swap_config->tam_pagina, mem_swap_config->tam_pagina);
       return contenido;
    }
    
    format_error_log("swap.c@leer_pagina_swap", "PID: %d - No tiene la pagina %d informada", pid, pagina); 
    return NULL; 
}


void eliminar_archivo_directorio_swap(uint32_t pid){


    char* path = string_from_format ("%s/%d.swap",mem_swap_config->path_swap, pid);
    format_debug_log("swap.c@eliminar_archivo_directorio_swap", "Se genera el PATH + ARCHIVO a eliminar: %s",path);

    if(remove(path)==0){ // Eliminamos el archivo. Retorna 0 en caso de exito
    	format_info_log("eliminar_archivo_swap.c@eliminar_archivo_swap","[ELIMINACION EXITOSA DE ARCHIVO SWAP] - Proceso: %d", pid);

    }else{
        format_warning_log("eliminar_archivo_swap.c@eliminar_archivo_swap","[ELIMINACION FALLIDA DE ARCHIVO SWAP] - Proceso: %d", pid);
    }

	free(path);

}
