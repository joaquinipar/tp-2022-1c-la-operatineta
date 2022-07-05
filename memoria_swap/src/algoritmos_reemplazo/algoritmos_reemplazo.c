#include "../../include/algoritmos_reemplazo/algoritmos_reemplazo.h"

uint32_t correr_algoritmo_reemplazo(uint32_t pid) {

    switch(mem_swap_config->algoritmo_reeemplazo){
        case(CLOCK): {
            format_debug_log("algoritmos_reemplazo.c@correr_algoritmo_reemplazo","Corriendo algoritmo CLOCK para el proceso PID: %i", pid);
            return manejar_clock(pid);
            break;
        }
        case(CLOCKMOD):{
            format_debug_log("algoritmos_reemplazo.c@correr_algoritmo_reemplazo","Corriendo algoritmo CLOCK-M para el proceso PID: %i", pid);
            return manejar_clock_modificado(pid);
            break;
        }
        default:{
            error_log("algoritmos_reemplazo.c@correr_algoritmo_reemplazo", "ERROR! Tipo de algoritmo de reemplazo no reconocido!");
            break;
        }
    }
    return -1;
}

uint32_t manejar_clock(uint32_t pid) {

    // Es la posición del puntero del proceso pid
    int puntero_clk = obtener_puntero_clock(pid);

    // El i representa la cantidad de marcos que voy recorriendo, no la posición que debo acceder.
    int i = 0;

    // El i_marco representa el marco a acceder. El modulo sirve para que cuando (imaginate que tengo 64 paginas),
    // llegue a la pagina 63, vaya a la 0, y siga recorriendo desde ahí.
    int i_marco;

    while (1) {


        for (i = 0; i < mem_ppal->cant_marcos; i++) {

            i_marco = (i + puntero_clk) % mem_ppal->cant_marcos;


            if (array_marcos[i_marco].pagina->nro_pagina != -1 && array_marcos[i_marco].pid == pid) {

                /*
                // debug
                _struct_esta_corrupto(array_marcos[i_marco].pagina->bit_uso, array_marcos[i_marco].pagina->bit_modificado);
                // debug end TODO manejar caso bits con valores incorrectos. que hago en ese caso?
                */


                // caso exito => bit uso = 0

                if (array_marcos[i_marco].pagina->bit_uso == 0) {

                    // Muevo puntero al proximo marco del proceso
                    mover_puntero_fija(i_marco, pid);

                    format_debug_log("algoritmos_reemplazo.c@manejar_clock", "PID %i Numero de pagina elegida: %i", pid, array_marcos[i_marco].pagina->nro_pagina);

                    // Retorno victima
                    return i_marco;
                }
                // caso perdonador => bit de uso = 1. Le pongo el bit de uso en 0
                else {
                    array_marcos[i_marco].pagina->bit_uso = 0;

                }
            }

            }

    } // end while

    return 1;
}

uint32_t manejar_clock_modificado(uint32_t pid) {
    // Es la posición del puntero del proceso pid
    int puntero_clk_mod = obtener_puntero_clock(pid);

    // Representa al modo, (0,0) | (0,1).
    int i_clock = 0;

    // El i representa la cantidad de marcos que voy recorriendo, no la posición que debo acceder.
    int i = 0;

    // El i_marco representa el marco a acceder. El modulo sirve para que cuando (imaginate que tengo 64 paginas),
    // llegue a la pagina 63, vaya a la 0, y siga recorriendo desde ahí.
    int i_marco;

    while (1) {


        for (i = 0; i < mem_ppal->cant_marcos; i++) {

            i_marco = (i + puntero_clk_mod) % mem_ppal->cant_marcos;

            if (array_marcos[i_marco].pagina->nro_pagina != -1 && array_marcos[i_marco].pid == pid) {

                // debug
                _struct_esta_corrupto(array_marcos[i_marco].pagina->bit_uso, array_marcos[i_marco].pagina->bit_modificado);
                // debug end TODO manejar caso bits con valores incorrectos. que hago en ese caso?

                if (i_clock == 0) {
                    // caso inicial, busco (0,0) y no seteo bits de uso

                    if (array_marcos[i_marco].pagina->bit_uso  == 0 &&
                        array_marcos[i_marco].pagina->bit_modificado == 0) {

                        // Muevo puntero al proximo marco del proceso
                        mover_puntero_fija(i_marco, pid);
                        // Retorno victima
                        return i_marco;
                    }
                }
                else {
                    // caso 2, busco (0,1) y seteo los bits de uso

                    if (
                            array_marcos[i_marco].pagina->bit_uso == 0 &&
                            array_marcos[i_marco].pagina->bit_modificado == 1) {

                        // Muevo puntero al proximo marco del proceso
                        mover_puntero_fija(i_marco, pid);
                        // Retorno victima
                        return i_marco;
                    }

                    // Seteo bit de uso en cero.
                    array_marcos[i_marco].pagina->bit_uso = 0;
                }
            } // end != 1
        } // end for

        i_clock = !i_clock; // Esto va a ser que cambie de modo (0,0) a (0,1).
    } // end while
}

// BUROCRACIA DE LA LISTA DE PUNTEROS DE CLOCK MODIFICADA - FIJA

// Inicialización

void iniciar_lista_punteros_clock(){

        punteros_procesos = list_create();
}

// Agregar puntero a lista de punteros de clock/clockmod

int agregar_puntero_nuevo_clock(uint32_t pid){

        if(punteros_procesos == NULL){

            error_log("algoritmos_reemplazo.c@agregar_puntero_nuevo_clock", "[ERROR] - No está inicializada la lista de punteros de clock");
            return -1;
        }

        bool pid_iguales(puntero_clock_modificado* puntero_struct) {
            return puntero_struct->pid == pid;
        }

        bool existe = list_any_satisfy(punteros_procesos, (void*) pid_iguales);

        if(!existe){

            puntero_clock_modificado* puntero_struct = malloc(sizeof(puntero_clock_modificado));

            puntero_struct->pid = pid;
            puntero_struct->marco_apuntado = get_marco_reservado_por_proceso(pid); // Asigna el primer marco reservado del proceso. ;)

            list_add(punteros_procesos,(void*) puntero_struct );

            debug_log("algoritmos_reemplazo.c@agregar_puntero_nuevo_clock", "Proceso agregado satisfactoriamente a la lista de punteros de Clock");
            return 1;
        }

        debug_log("algoritmos_reemplazo.c@agregar_puntero_nuevo_clock", "El proceso ya existe en la lista de punteros de Clock");

        return -1;


}

// Obtener puntero de clock

int obtener_puntero_clock(uint32_t pid){


    bool pid_iguales(puntero_clock_modificado* puntero_struct) { return puntero_struct->pid == pid; }

    puntero_clock_modificado* puntero_struct = (puntero_clock_modificado*) list_find(punteros_procesos, (void*) pid_iguales);

    if(puntero_struct != NULL){
        return puntero_struct->marco_apuntado;
    }

    error_log("algoritmos_reemplazo.c@obtener_puntero_clock", "ERROR - No está inicializada la lista de punteros de clock/modificado");
    return -1;

}


// destruir lista de punteros;

void destruir_lista_de_punteros_clock(){
    list_destroy_and_destroy_elements(punteros_procesos, (void*) free);
    info_log("algoritmos_reemplazo.c@destruir_lista_de_punteros_clock","Lista de punteros de clock destruida");
}

// Setear marco a un puntero de clock dado un PID
int setear_marco_a_puntero_clock(uint32_t pid, uint32_t marco_a_setear){

    bool pid_iguales(puntero_clock_modificado* puntero_struct) {
        return puntero_struct->pid == pid; }

    puntero_clock_modificado* puntero_struct = (puntero_clock_modificado*) list_find(punteros_procesos, (void*) pid_iguales);

    if(puntero_struct != NULL){
        puntero_struct->marco_apuntado = marco_a_setear;
        debug_log("setear_marco_a_puntero_clock_modificado@algoritmos_reemplazo.c", "Se setea marco a puntero de CLOCK/CLOCKMOD");
        return 1;
    }

    error_log("setear_marco_a_puntero_clock_modificado@algoritmos_reemplazo.c", "ERROR - No se encontro un puntero de clock modificado con ese pid. ¿Iniciaste la lista antes?");
    return -1;
}

void mover_puntero_fija(int marco, uint32_t pid){

    // Solo usar para Clock Modificado

    bool pid_iguales(puntero_clock_modificado* puntero_struct) { return puntero_struct->pid == pid; }

    puntero_clock_modificado* puntero_struct = (puntero_clock_modificado*) list_find(punteros_procesos, (void*) pid_iguales);


    uint32_t marco_inicial = get_primer_marco_usado(pid);

    if(marco_inicial != -1 && puntero_struct != NULL){

        uint32_t marco_siguiente = get_proximo_marco_del_proceso( pid,marco);

        if(marco_siguiente != -1){

            puntero_struct->marco_apuntado = marco_siguiente;
            return;
        }
        else {
            // fallo
            error_log("algoritmos.c@mover_puntero_fija", "El marco que me pasaste no está dentro del espacio de direcciones del proceso");
            return;
        }
    }
    else{
        error_log("algoritmos.c@mover_puntero_fija", "No se encontró el marco inicial del proceso o el puntero del proceso. ¿Allocaste el proceso previamente?");
        return;
    }

    // you shouldn't be here
    return;
}

uint32_t get_proximo_marco_del_proceso(uint32_t pid, uint32_t marco_actual){

    int encontrar_proceso(puntero_clock_modificado* puntero) {
        return puntero->pid == pid;
    }

    puntero_clock_modificado* puntero = list_find(punteros_procesos, (void*) &encontrar_proceso ); // no hacer free

    int i_marco = marco_actual;



    for (int i = 0; i < mem_ppal->cant_marcos; i++) {

        i_marco = (i + puntero->marco_apuntado) % mem_ppal->cant_marcos;


        if (array_marcos[i_marco].pagina->nro_pagina != -1 && array_marcos[i_marco].estado == 1 &&
            array_marcos[i_marco].pid == pid && array_marcos[i_marco].pagina->bit_presencia == 1 &&
            i_marco != marco_actual
            ) {

            return i_marco;
        }

    } // end for

    error_log("get_proximo_marco_del_proceso@algoritmos.c","No se encontró víctima. ¿Estás seguro de que hay otros marcos disponibles para reemplazo?");
    return -1;
}

uint32_t get_primer_marco_usado(uint32_t pid) {
    //
    int marco;
    int marco_asignado = -1;
    for (marco = 0; marco < mem_ppal->cant_marcos; marco++) {
        if ((array_marcos[marco].estado == 1) &&
            (array_marcos[marco].pid == pid)) {
            marco_asignado = marco;
            break;
        }
    }
    char *msg = string_from_format("El primer marco reservado sin pagina asignada libre del array de marcos es: %d", marco_asignado);
    trace_log("algoritmos_reemplazo.c@get_primer_marco_usado", msg);
    free(msg);
    ////
    return marco_asignado;
}


uint32_t get_marco_reservado_por_proceso(uint32_t pid) {

    int marco;
    int marco_asignado = -1;
    for (marco = 0; marco < mem_swap_config->marcos_por_proceso; marco++) {
        if ((array_marcos[marco].estado == 1) &&
            (array_marcos[marco].pid == pid) && (array_marcos[marco].pagina->nro_pagina == -1)) {
            marco_asignado = marco;
            break;
        }
    }
    char *msg = string_from_format("[1ER MARCO RESERVADO SIN PAGINA ASIGNADA]: %d", marco_asignado);
    debug_log("algoritmos_reemplazo.c@get_marco_reservado_por_proceso", msg);
    free(msg);

    return marco_asignado;
}

int _struct_esta_corrupto(uint32_t bit_uso, uint32_t bit_modificado){

    if ( !(bit_uso == 0 || bit_uso == 1)  ||
         !(bit_modificado == 0 || bit_modificado == 1) ) {

        error_log("algoritmos_reemplazo.c@_struct_esta_corrupto","ERROR!! Bit de uso o Bit de modificado con valor inválido");
        format_error_log("algoritmos_reemplazo.c@_struct_esta_corrupto","Esperaba valores (0 | 1) y obtuve: BIT USO:%i BIT MODIFICADO:%i",bit_uso, bit_modificado);

        return 1;
    }
    return 0;
}

int buscar_y_reemplazar(uint32_t pid){

    uint32_t marco_victima = correr_algoritmo_reemplazo(pid);
    ///
    uint32_t pagina_actual = array_marcos[marco_victima].pagina->nro_pagina;
    format_info_log("algoritmos_reemplazo.c@buscar_y_reemplazar", "Victima seleccionada marco: %i pagina: %i", marco_victima, pagina_actual);
    format_info_log("algoritmos_reemplazo.c@buscar_y_reemplazar", "Se mueve el puntero clock del pid: %i a la posición: %i", pid, obtener_puntero_clock(pid));
    ///

    if (array_marcos[marco_victima].pagina->bit_modificado == 1){
        // Si tiene el bit de modificado en 1 y nuestra intención es liberar el marco victima, debemos bajarlo a swap
        // para no perderlo permanentemente.
        int estado = escribir_pagina_swap(pid, marco_victima);

        if(estado == -1){
            return -1;
        }
    }

    // Voy a la pagina (de la tabla) y le pongo los bits en 0 y le saco el marco.
    array_marcos[marco_victima].pagina->bit_presencia = 0;
    array_marcos[marco_victima].pagina->bit_uso = 0;
    array_marcos[marco_victima].pagina->marco = -1;


    return marco_victima;
}
