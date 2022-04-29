#include "../include/memoria_principal_helper.h"

/**
* @NAME: buscar_tabla_1er_nivel_de_un_proceso
* @DESC: Busca la tabla de 1er nivel de un proceso en la lista de tablas de primer nivel.
 * ATENCIÓN: No se debe liberar lo que retorna esta función, ya que está en una lista.
*/
entrada_1er_nivel_t* buscar_tabla_1er_nivel_de_un_proceso(int pid){

    int _encontrar_proceso_en_tabla_global(entrada_1er_nivel_t* tabla){
        //format_info_log("memoria_principal_helper.c@buscar_proceso_en_lista_tablas_1er_nivel","%i %i", pid, tabla->pid);
        return pid == tabla->pid;
    }

    return list_find(lista_tablas_1er_nivel, (void*)&_encontrar_proceso_en_tabla_global);
}

/**
* @NAME: buscar_proceso_en_lista_tablas_1er_nivel
* @DESC: Busca la tabla de un proceso en la lista de tablas de 1er nivel. Retorna 1 si lo encontro, 0 si no lo encontro.
*/
int proceso_presente_en_lista_tabla_1er_nivel(int pid){

    int _encontrar_proceso_en_tabla_global(entrada_1er_nivel_t* tabla){
        //format_info_log("memoria_principal_helper.c@buscar_proceso_en_lista_tablas_1er_nivel","%i %i", pid, tabla->pid);
        return pid == tabla->pid;
    }

    return list_find(lista_tablas_1er_nivel, (void*)&_encontrar_proceso_en_tabla_global) != NULL;
}

entrada_1er_nivel_t* buscar_proceso_en_lista_tablas_2do_nivel(int pid){

    int _encontrar_proceso_en_tabla_global(tabla_2do_nivel_t* tabla){
        format_info_log("memoria_principal_helper.c@buscar_proceso_en_lista_tablas_1er_nivel","%i %i", pid, tabla->pid);
        return pid == tabla->pid;
    }

    return list_find(lista_tablas_1er_nivel, (void*)&_encontrar_proceso_en_tabla_global);
}

entrada_1er_nivel_t* crear_tabla_1er_nivel_inicializada(uint32_t pid){
    entrada_1er_nivel_t* nueva_tabla = malloc(mem_swap_config->entradas_por_tabla*sizeof (entrada_1er_nivel_t));
    nueva_tabla->pid = pid;
    nueva_tabla->nro_tabla_2do_nivel = -1;
    return nueva_tabla;
}

/**
* @NAME: inicializar_tabla_1er_nivel
* @DESC: Inicializa las entradas de una tabla de primer nivel. Le asigna el pid y pone las tablas de 2do nivel en -1
*/
void inicializar_tabla_1er_nivel(entrada_1er_nivel_t* tabla_1er_nivel, uint32_t pid) {
    for(int i=0; i < mem_swap_config->entradas_por_tabla; i++) {
        (tabla_1er_nivel+i)->pid = pid;
        (tabla_1er_nivel+i)->nro_tabla_2do_nivel = -1;
    }
}

int agregar_entrada_tabla_1er_nivel(entrada_1er_nivel_t* tabla_1er_nivel, uint32_t nro_tabla_2do_nivel) {

    for(int i=0; i < mem_swap_config->entradas_por_tabla; i++) {

        if(tabla_1er_nivel[i].nro_tabla_2do_nivel == -1) {
            // Si entra es porque puede usar esa entrada
            tabla_1er_nivel[i].nro_tabla_2do_nivel = nro_tabla_2do_nivel;
            return i; // devuelvo numero de entrada donde se guardo
        }
    }
    error_log("agregar_entrada_tabla_1er_nivel@memoria_principal_helper.c", "No se encontro una entrada disponible. ¿Se incializo la tabla de 1er nivel?");
    return -1;
}

tabla_2do_nivel_t* crear_entradas_tabla_2do_nivel(uint32_t pid) {
    tabla_2do_nivel_t* una_tabla_2do_nivel = malloc(sizeof(tabla_2do_nivel_t));
    una_tabla_2do_nivel->pid = pid;
    una_tabla_2do_nivel->contenido_tabla_2do_nivel = malloc(sizeof(pagina_2do_nivel_t)*mem_swap_config->entradas_por_tabla);

    for (int i = 0; i < mem_swap_config->entradas_por_tabla; i++) {
        (una_tabla_2do_nivel->contenido_tabla_2do_nivel)[i].nro_pagina = -1;
        (una_tabla_2do_nivel->contenido_tabla_2do_nivel)[i].marco = -1;
        (una_tabla_2do_nivel->contenido_tabla_2do_nivel)[i].bit_presencia = -1;
        (una_tabla_2do_nivel->contenido_tabla_2do_nivel)[i].bit_modificado = -1;
        (una_tabla_2do_nivel->contenido_tabla_2do_nivel)[i].bit_uso = -1;
        (una_tabla_2do_nivel->contenido_tabla_2do_nivel)[i].time_ref = -1;
    }
    return una_tabla_2do_nivel;
}

//entrada_1er_nivel_t* crear_tabla_1er_nivel(int pid, int tamanio){

int32_t obtener_cant_paginas_a_utilizar(int32_t tamanio_total_size) {

    int32_t cant_total_page = ceil((float)tamanio_total_size / (float)mem_swap_config->tam_pagina);
    format_debug_log(".c@obtener_cant_paginas_a_utilizar", "La cantidad de paginas a utilizar son: %d", cant_total_page);
    return cant_total_page;
}

int buscar_proceso_en_lista_tablas_1er_nivel(uint32_t pid, uint32_t nro_entrada_1er_nivel){

    int _encontrar_proceso_en_tabla_global(entrada_1er_nivel_t* tabla){
        //format_info_log("memoria_principal_helper.c@buscar_proceso_en_lista_tablas_1er_nivel","%i %i", pid, tabla->pid);
        return pid == tabla->pid;
    }

    return list_find(lista_tablas_1er_nivel, (void*)&_encontrar_proceso_en_tabla_global) != NULL;
}

