#include "../../include/swap/swap.h"

archivo_pid_t* crear_archivo_pid(int pid, uint32_t tam_proceso){
    archivo_pid_t* archivo = malloc(sizeof(archivo_pid_t));
    archivo->pid = pid;
    archivo->path_archivo = obtener_ruta_swap_proceso(pid);
    archivo->tam_proceso = tam_proceso;
    crear_archivo_swap(archivo);
    int cantidad_marcos = mem_swap_config->tam_memoria / mem_swap_config->tam_pagina;
    archivo->array_marcos_virtual_del_proceso = malloc(cantidad_marcos * sizeof(marco_virtual_t));

    return archivo;
}

void iniciar_swap() {
    list_archivos_swap = list_create();
}

void destruir_list_swap(){
    list_destroy_and_destroy_elements(list_archivos_swap, (void*)free); 
}

