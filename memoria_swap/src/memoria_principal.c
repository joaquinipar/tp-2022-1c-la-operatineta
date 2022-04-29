#include "../include/memoria_principal.h"


mem_principal_t *crear_mem_principal() {
  mem_principal_t *mp = malloc(sizeof(mem_principal_t));
  mp->memoria_principal = NULL;
  mp->cant_marcos = 0;
  info_log("memoria_principal.c@crear_mem_principal", "Se creo la estructura de Memoria Principal");
  return mp;
}

void destruir_estructura_mem_principal() {
  free(mem_ppal->memoria_principal);
  free(mem_ppal);
  info_log("memoria_principal.c@destruir_estructura_mem_principal", "Se destruyo la estructura de Memoria Principal");
}


void iniciar_memoria_principal(){
  info_log("memoria_principal.c@iniciar_memoria_principal", "Comienza Inicio de Memoria Principal");
  mem_ppal = crear_mem_principal();
  mem_ppal->memoria_principal = malloc(mem_swap_config->tam_memoria);

  if (mem_ppal->memoria_principal == NULL) {
    error_log("memoria_principal.c@iniciar_memoria_principal", "Fallo malloc de la memoria_principal");
  }
  memset(mem_ppal->memoria_principal, '\0', mem_swap_config->tam_memoria);
  // Coloco toda la memoria en mem->cant_memoria_ppal_libre = '\0'
  format_info_log("memoria_principal.c@iniciar_memoria_principal", "Cantidad de memoria libre es: %d", mem_swap_config->tam_memoria);

  //iniciar_array_mem(); ya lo estas haciendo en el crear_mem_principal()

}



void crear_array_mem() {

  info_log("memoria_principal.c@crear_array_mem", "Comienza la inicializacion de la estructura adm Array de Marcos");

  mem_ppal->cant_marcos = mem_swap_config->tam_memoria / mem_swap_config->tam_pagina;
  //Calcula la cant de marcos de la mem ppal
  array_marcos = (marco_t *)malloc(mem_ppal->cant_marcos * sizeof(marco_t));

  for (int i = 0; i < mem_ppal->cant_marcos; ++i) {
// Seteo todo el array en libre. 0-> libre y demas en -1
    array_marcos[i].estado = 0;   /* Estado -> LIBRE - Estado 1 -> Ocupado */
    array_marcos[i].pid = -1;
    (array_marcos[i].pagina) = (pagina_2do_nivel_t *)malloc(sizeof(pagina_2do_nivel_t));
    (array_marcos[i].pagina)->nro_pagina = -1;
    array_marcos[i].pagina->bit_presencia = -1;
    array_marcos[i].pagina->bit_modificado = -1;
    array_marcos[i].pagina->bit_uso = -1;
    array_marcos[i].pagina->time_ref = -1;
  }
  debug_log("memoria_principal.c@crear_array_mem", "Finaliza la inicializacion de la estructura adm Array de Marcos");
}

void destruir_array_mem() {
  for (int i = 0; i < mem_ppal->cant_marcos; i++) {
    free((array_marcos[i].pagina));
  }
  free(array_marcos);
  debug_log("memoria_principal.c@destruir_array_mem", "Se destruyo la estructura Array Marcos");
}



void crear_array_1er_table() {

  info_log("memoria_principal.c@crear_array_mem", "Comienza la inicializacion de la estructura adm Array de Marcos");

  for (int i = 0; i < mem_swap_config->entradas_por_tabla; ++i) {
// Seteo todo el array en libre. 0-> libre y demas en -1

  }
  debug_log("memoria_principal.c@crear_array_mem", "Finaliza la inicializacion de la estructura adm Array de Marcos");
}

void iniciar_listas_globales_de_tablas() {
    lista_tablas_1er_nivel = list_create();
    lista_tablas_2do_nivel = list_create();
    trace_log("proceso.c@iniciar_listas_globales_de_tablas","Se inicia la lista global de tablas de 1er nivel");
    trace_log("proceso.c@iniciar_listas_globales_de_tablas","Se inicia la lista global de tablas de 2do nivel");
}

void inicializar_tablas() {

    lista_tablas_1er_nivel = list_create();
    lista_tablas_2do_nivel = list_create();
}
