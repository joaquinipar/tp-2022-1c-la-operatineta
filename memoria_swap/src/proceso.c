#include "../include/proceso.h"
 
entrada_1er_nivel_t* buscar_tabla_1er_nivel_de_un_proceso(uint32_t pid){

    int _encontrar_proceso_en_tabla_global(entrada_1er_nivel_t* tabla){
        return pid == tabla->pid;
    }

    return list_find(lista_tablas_1er_nivel, (void*)&_encontrar_proceso_en_tabla_global);
}


int proceso_presente_en_lista_tabla_1er_nivel(uint32_t pid){

    int _encontrar_proceso_en_tabla_global(entrada_1er_nivel_t* tabla){
        return pid == tabla->pid;
    }

    return list_find(lista_tablas_1er_nivel, (void*)&_encontrar_proceso_en_tabla_global) != NULL;
}

entrada_1er_nivel_t*  iniciar_tabla_1er_nivel(uint32_t pid){
    entrada_1er_nivel_t* una_tabla_1er_nivel = malloc(mem_swap_config->entradas_por_tabla*sizeof(entrada_1er_nivel_t));
    for(int entrada=0; entrada < mem_swap_config->entradas_por_tabla; entrada++) {
        una_tabla_1er_nivel[entrada].pid = pid;
        una_tabla_1er_nivel[entrada].nro_tabla_2do_nivel = -1;
    }
    format_info_log("proceso.c@iniciar_tabla_1er_nivel", "Se inicia tabla de 1er nivel - Proceso: %d", pid);
    return una_tabla_1er_nivel; 
}


int32_t obtener_cant_paginas_a_utilizar(int32_t tamanio_total_size) {

    int32_t cant_total_page = ceil((float)tamanio_total_size / (float)mem_swap_config->tam_pagina);
    format_debug_log(".c@obtener_cant_paginas_a_utilizar", "La cantidad de paginas a utilizar son: %d", cant_total_page);
    return cant_total_page;
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


void gestionar_tabla_2do_nivel(uint32_t cant_tablas_2do_nivel, entrada_1er_nivel_t* tabla_1er_nivel, uint32_t pid){

    for(int i=0;i<cant_tablas_2do_nivel;i++){

        tabla_2do_nivel_t* nuevo_tabla_2do_nivel = crear_entradas_tabla_2do_nivel(pid);

        int posicion_lista_tablas_2do_nivel = list_add(lista_tablas_2do_nivel, nuevo_tabla_2do_nivel);

        // Busco en que posición de la lista global de tablas de 2do nivel se guardo, y guardo la posición en la tabla
        // de primer nivel.

        agregar_entrada_tabla_1er_nivel(tabla_1er_nivel, posicion_lista_tablas_2do_nivel);

        /*
         * TODO Pensamiento:
         * Ponele que tengo que guardar 4 entradas de tabla de segundo nivel
         * ¿Me puede quedar 2 entradas (por ej..) en una tabla de segundo nivel numero 5 y otras 2 en la tabla de numero 6?
         * Esto pasaria porque en la tabla 5 me quede sin entradas.
         * Posible solución: Hacer un wrapper de list_add que se fije el list_size, si no nos alcanza vamos a tener que usar
         * otra tabla más y se encarga el. ni nos enteramos nosotros desde aca
         */
    }

}

uint32_t inicio_proceso(uint32_t pid, uint32_t tamanio){

    if(!proceso_presente_en_lista_tabla_1er_nivel(pid)){

        admitir_proceso_en_swap(pid, tamanio); 

        uint32_t cant_paginas_a_utilizar = obtener_cant_paginas_a_utilizar(tamanio);
        //uint32_t cant_entradas_a_usar_1er_nivel = cant_tablas_2do_nivel;
        uint32_t cant_tablas_2do_nivel = cant_paginas_a_utilizar % mem_swap_config->entradas_por_tabla;
        format_debug_log("proceso.c@admitir_proceso", "Proceso: %d  -- Cantidad de tablas de 2do niveles necesarias: %d", pid, cant_tablas_2do_nivel); 
        
        entrada_1er_nivel_t* tabla_1er_nivel = iniciar_tabla_1er_nivel(pid); 
        gestionar_tabla_2do_nivel(cant_tablas_2do_nivel,tabla_1er_nivel, pid); 
        int posicion_tabla_1er_nivel_en_lista_global = list_add(lista_tablas_1er_nivel,tabla_1er_nivel);
        format_info_log("proceso.c@admitir_proceso", "Proceso: %d  admitido en Memoria- Nro de tabla 1er nivel asignada: %d", pid, posicion_tabla_1er_nivel_en_lista_global); 
        return posicion_tabla_1er_nivel_en_lista_global; 

    }
    return -1; 
}


uint32_t obtener_marco_de_tabla_2do_nivel(uint32_t pid, uint32_t nro_tabla_2do_nivel, uint32_t nro_pagina) {

    tabla_2do_nivel_t* tabla_2do_nivel = list_get(lista_tablas_2do_nivel, nro_tabla_2do_nivel);

    if(tabla_2do_nivel == NULL){
        format_error_log("memoria_api.c@obtener_marco_de_tabla_2do_nivel","%s%d", "No se encontro el numero de tabla: ", nro_tabla_2do_nivel);
        return -1;
    }
    else if (tabla_2do_nivel[nro_pagina].pid != pid) {
        format_error_log("memoria_api.c@obtener_marco_de_tabla_2do_nivel", "%s%d%s%d",
                         "Accediste a la tabla de segundo nivel del proceso: ",
                         tabla_2do_nivel[nro_pagina].pid,
                         " pero esperaba el proceso: ",
                         pid);
    }

    return tabla_2do_nivel[nro_pagina].contenido_tabla_2do_nivel->marco;
}

uint32_t buscar_nro_tabla_2do_nivel (uint32_t pid, uint32_t nro_tabla_1er_nivel, uint32_t nro_entrada_1er_nivel) {

    entrada_1er_nivel_t * tabla_1er_nivel = list_get(lista_tablas_1er_nivel, nro_tabla_1er_nivel); // no hacer free
    if(tabla_1er_nivel == NULL){
        format_error_log("memoria_api.c@buscar_nro_tabla_2do_nivel", "%s%d%s", "PID: ", pid, ", no está presente en la lista de tablas de paginas");
        return -1;
    }

    return tabla_1er_nivel[nro_entrada_1er_nivel].nro_tabla_2do_nivel;
}