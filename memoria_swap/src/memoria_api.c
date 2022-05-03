#include "../include/memoria_api.h"

uint32_t inicio_proceso(uint32_t pid, uint32_t tamanio){
    info_log("memoria_api.c@inicio_struct_adm", "Se inicia las estructuras administrativas del proceso"); 

    // todo checkear si por error me mandaron el proceso dos veces. checkear si ya esta

    archivo_pid_t* un_arch_swap = crear_archivo_pid(pid, tamanio);
    list_add(list_archivos_swap, un_arch_swap); 

    uint32_t cant_paginas_a_utilizar = obtener_cant_paginas_a_utilizar(tamanio);
    uint32_t cant_tablas_2do_nivel = cant_paginas_a_utilizar % mem_swap_config->entradas_por_tabla;

    //uint32_t cant_entradas_a_usar_1er_nivel = cant_tablas_2do_nivel;

    entrada_1er_nivel_t* una_tabla_1er_nivel = malloc(mem_swap_config->entradas_por_tabla*sizeof(entrada_1er_nivel_t));
    inicializar_tabla_1er_nivel(una_tabla_1er_nivel, pid);

    for(int i=0;i<cant_tablas_2do_nivel;i++){

        tabla_2do_nivel_t* nuevo_tabla_2do_nivel = crear_entradas_tabla_2do_nivel(pid);

        int posicion_lista_tablas_2do_nivel = list_add(lista_tablas_2do_nivel, nuevo_tabla_2do_nivel);

        // Busco en que posición de la lista global de tablas de 2do nivel se guardo, y guardo la posición en la tabla
        // de primer nivel.

        agregar_entrada_tabla_1er_nivel(una_tabla_1er_nivel, posicion_lista_tablas_2do_nivel);

        /*
         * TODO Pensamiento:
         * Ponele que tengo que guardar 4 entradas de tabla de segundo nivel
         * ¿Me puede quedar 2 entradas (por ej..) en una tabla de segundo nivel numero 5 y otras 2 en la tabla de numero 6?
         * Esto pasaria porque en la tabla 5 me quede sin entradas.
         * Posible solución: Hacer un wrapper de list_add que se fije el list_size, si no nos alcanza vamos a tener que usar
         * otra tabla más y se encarga el. ni nos enteramos nosotros desde aca
         */
    }

    int posicion_tabla_1er_nivel_en_lista_global = list_add(lista_tablas_1er_nivel,una_tabla_1er_nivel);

    // TODO agregar las paginas a utilizar con los marcos libres condicional si esta lleno o no bla bla bla
    // TODO Checkear errores, ¿Ocurrió algún error en el medio del proceso?

    return posicion_tabla_1er_nivel_en_lista_global; // TODO te parece bien este return?
}

uint32_t buscar_nro_tabla_2do_nivel (uint32_t pid, uint32_t nro_tabla_1er_nivel, uint32_t nro_entrada_1er_nivel) {

    entrada_1er_nivel_t * tabla_1er_nivel = list_get(lista_tablas_1er_nivel, nro_tabla_1er_nivel); // no hacer free
    if(tabla_1er_nivel == NULL){
        format_error_log("memoria_api.c@buscar_nro_tabla_2do_nivel", "%s%d%s", "PID: ", pid, ", no está presente en la lista de tablas de paginas");
        return -1;
    }

    return tabla_1er_nivel[nro_entrada_1er_nivel].nro_tabla_2do_nivel;
}

void* leer(uint32_t direccion_fisica) {
    void* contenido = malloc(sizeof (uint32_t));
    memcpy( contenido ,mem_ppal->memoria_principal + direccion_fisica, sizeof(uint32_t));
    return contenido;
}

void escribir(uint32_t direccion_fisica, void* contenido){

    memcpy(mem_ppal->memoria_principal + direccion_fisica, contenido, sizeof(uint32_t));
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