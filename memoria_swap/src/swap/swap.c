#include "../../include/swap/swap.h"

void iniciar_swap() {
    debug_log("swap.c@iniciar_swap", "Comienza el inicio de la lista global de archivos de swap por proceso"); 
    list_archivos_swap = list_create();
    //debug_log("swap.c@iniciar_swap", "Finaliza el inicio de la lista global de archivos de swap por proceso"); 
}

void destruir_list_swap(){

    void _eliminar_ficheros(archivo_pid_t* proceso_swap){
    	eliminar_archivo_directorio_swap(proceso_swap->pid);
    }

	list_iterate(list_archivos_swap,(void*) _eliminar_ficheros);

    list_destroy_and_destroy_elements(list_archivos_swap, (void*)free); 
    debug_log("swap.c@iniciar_swap", "Se destruye la lista global de archivos de swap por proceso"); 
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
        /* Caso: El tamaño es menor de una pagina, o es una pagina justa */
        return 1;
    } else if (tam_proceso % mem_swap_config->tam_pagina == 0) {
        return resultado;
    } else {
        /* Caso: El tamaño es mayor a una pagina y hay resto */
        return (uint32_t)resultado + 1;
    }
}



void admitir_proceso_en_swap(uint32_t pid, uint32_t tamanio){
    format_info_log("swap.c@admitir_proceso_en_swap", "Admision en Swap - Proceso: %d  -- Tamaño %d", pid, tamanio); 
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


  if (proceso_a_eliminar == NULL) {
      error_log("swap.c@eliminar_proceso_en_lista_global_swap", "[ERROR] - No se pudo eliminar proceso de la lista global de swap"); 
    return 0;
  } 

    format_debug_log("swap.c@eliminar_proceso_en_lista_global_swap", "Se elimino el proceso %d de la lista global de swap", pid);
    return 1;
}

int escribir_pagina_swap(uint32_t pid, uint32_t marco){

    archivo_pid_t* archivo = get_proceso_swap(pid);

    if(archivo == NULL){
        format_error_log("swap.c@escribir_pagina_swap", "El proceso %i no tiene archivo swap creado.", pid);
        return -1;
    }

    usleep(mem_swap_config->retardo_swap);

    memcpy(archivo->area_archivo_swap + marco * mem_swap_config->tam_pagina,
           mem_ppal->memoria_principal + marco * mem_swap_config->tam_pagina,
           mem_swap_config->tam_pagina);

    format_info_log("swap.c@escribir_pagina_swap", "PID: %i Marco: %i escrito en SWAP correctamente! DIR: %i",pid,marco, marco * mem_swap_config->tam_pagina);


    if( msync( archivo->area_archivo_swap, archivo->tam_proceso , MS_SYNC) < 0){ // ¿Está bien ese tamaño no?
        format_error_log("swap.c@escribir_pagina_swap","(VICTIMA)(msync) PID:%i Error al volcar los cambios a SWAP",(int)archivo->pid);
    }


    return 1;
}

void* leer_pagina_swap(uint32_t pid, uint32_t marco){

    archivo_pid_t* archivo = get_proceso_swap(pid);

    if(archivo == NULL){
        format_error_log("swap.c@escribir_pagina_swap", "El proceso %i no tiene archivo swap creado.", pid);
        return NULL;
    }

    usleep(mem_swap_config->retardo_swap);

    void* contenido = malloc(mem_swap_config->tam_pagina);

    memcpy(contenido, archivo->area_archivo_swap + marco * mem_swap_config->tam_pagina, mem_swap_config->tam_pagina);

    return contenido;
}


void eliminar_archivo_directorio_swap(uint32_t pid){

    //Eliminar archivo de directorio swap.

    char *path =  string_from_format ("%s/%d.swap",mem_swap_config->path_swap, pid);
    format_debug_log("swap.c@eliminar_archivo_directorio_swap", "Se genera el PATH + ARCHIVO a eliminar: %s",path);

    if(remove(path)==0){ // Eliminamos el archivo
    	format_debug_log("eliminar_archivo_swap.c@eliminar_archivo_swap","[ELIMINACION EXITOSA DE ARCHIVO SWAP] - Proceso: %d", pid);
    	//return 0;

    }else{
        format_warning_log("eliminar_archivo_swap.c@eliminar_archivo_swap","[ELIMINACION FALLIDA DE ARCHIVO SWAP] - Proceso: %d", pid);
    	//return -1;
    }



}
