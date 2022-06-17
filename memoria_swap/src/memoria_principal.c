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
  debug_log("memoria_principal.c@iniciar_memoria_principal", "Comienza Inicio de Memoria Principal");
  mem_ppal = crear_mem_principal();
  mem_ppal->memoria_principal = malloc(mem_swap_config->tam_memoria);

  if (mem_ppal->memoria_principal == NULL) {
    error_log("memoria_principal.c@iniciar_memoria_principal", "Fallo malloc de la memoria_principal");
  }
  memset(mem_ppal->memoria_principal, '\0', mem_swap_config->tam_memoria);
  // Coloco toda la memoria en mem->cant_memoria_ppal_libre = '\0'
  format_debug_log("memoria_principal.c@iniciar_memoria_principal", "Cantidad de memoria libre es: %d", mem_swap_config->tam_memoria);

  crear_array_mem(); 
  debug_log("memoria_principal.c@iniciar_memoria_principal", "Finaliza Inicio de Memoria Principal");

}

void crear_array_mem() {

  debug_log("memoria_principal.c@crear_array_mem", "Comienza la inicializacion de la estructura adm Array de Marcos");

  mem_ppal->cant_marcos = mem_swap_config->tam_memoria / mem_swap_config->tam_pagina;
  //Calcula la cant de marcos de la mem ppal
  array_marcos = (marco_t *)malloc(mem_ppal->cant_marcos * sizeof(marco_t));

  for (int i = 0; i < mem_ppal->cant_marcos; ++i) {
// Seteo todo el array en libre. 0-> libre y demas en -1
    array_marcos[i].estado = 0;   /* Estado 0-> LIBRE - Estado 1 -> Ocupado */
    array_marcos[i].pid = -1;
    (array_marcos[i].pagina) = (pagina_2do_nivel_t *)malloc(sizeof(pagina_2do_nivel_t));
    (array_marcos[i].pagina)->nro_pagina = -1;
    array_marcos[i].pagina->bit_presencia = -1;
    array_marcos[i].pagina->bit_modificado = -1;
    array_marcos[i].pagina->bit_uso = -1;
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

void escribir_contenido_en_marco_MP(uint32_t new_pid, pagina_2do_nivel_t *new_page, uint32_t frame_a_escribir, int new_state) {

  array_marcos[frame_a_escribir].pid = new_pid;
  array_marcos[frame_a_escribir].pagina = new_page;
  array_marcos[frame_a_escribir].estado = new_state;
  format_debug_log("memoria_principal.c@escribir_contenido_en_marco_MP", "Escritura de Marco -> Marco %d -- Proceso: %d -- Pagina: %d -- Estado: %d", frame_a_escribir, array_marcos[frame_a_escribir].pid, array_marcos[frame_a_escribir].pagina->nro_pagina,array_marcos[frame_a_escribir].estado);
}

int marco_presente_MP(uint32_t pagina_buscada, uint32_t pid) {
  
  int marco;
  for (marco = 0; marco < mem_ppal->cant_marcos; marco++) {
    if (array_marcos[marco].pagina->nro_pagina == pagina_buscada &&
        array_marcos[marco].pid == pid &&
        array_marcos[marco].estado == 1 && array_marcos[marco].pagina->bit_presencia == 1) {
      format_debug_log("memoria_principal.c@marco_presente_MP", "La pagina %d del proceso %d se encuentra en MP", pagina_buscada, pid);
      
      return 1;
    }
  }
  format_error_log("memoria_principal.c@marco_presente_MP", "[ERROR] - NO se encuentra La pagina %d del proceso %d  en MP", pagina_buscada, pid); 
  return 0;
}

// Esta funcion te devuelve el marco donde se encuentra la pagina correspondiente a un proceso.
int buscar_marco_en_MP(uint32_t pagina_buscada, uint32_t pid) {

  if (marco_presente_MP(pagina_buscada, pid) == 1) {
    int marco;
    for (marco = 0; marco < mem_ppal->cant_marcos; marco++) {
      if ((array_marcos[marco].pagina->nro_pagina == pagina_buscada) && (array_marcos[marco].pid == pid)) {

        format_debug_log("memoria_principal.c@buscar_marco_en_MP", "La pagina %d del proceso %d se encuentra en MP en la marco Nro. %d",pagina_buscada, pid, marco);
        return marco;
      }
    }
  }

  format_error_log("memoria_principal.c@buscar_marco_en_MP", "[ERROR] - NO se encuentra La pagina %d del proceso %d  en  MP", pagina_buscada, pid);
  return -1;
}

uint32_t get_marco_MP(uint32_t pagina_buscada, uint32_t pid_buscado) {

  int marco_buscado = buscar_marco_en_MP(pagina_buscada, pid_buscado);
  format_debug_log("memoria_principal.c@get_marco_MP", "El numero de marco asignado a la pagina %d del proceso %d es:%d", pagina_buscada, pid_buscado, marco_buscado);
  return marco_buscado;
}

int calcular_marcos_ocupados_por_proceso(uint32_t pid) {
  int marco;
  int cant_marcos_ocupados_por_proceso = 0;
  for (marco = 0; marco < mem_ppal->cant_marcos; marco++) {

    if ((array_marcos[marco].estado == 1) && (array_marcos[marco].pid == pid && array_marcos[marco].pagina->nro_pagina != -1)) {
      cant_marcos_ocupados_por_proceso++;
    }
  }
  format_debug_log("memoria_principal.c@calcular_marcos_ocupados_por_proceso", "Marcos ocupados:%i -- Proceso: %i", cant_marcos_ocupados_por_proceso, (int)pid);
  return cant_marcos_ocupados_por_proceso;
}

bool tiene_marcos_reservados_libres(uint32_t pid) {
  int marcos_ocupados_por_proceso = calcular_marcos_ocupados_por_proceso(pid);
  if (marcos_ocupados_por_proceso < mem_swap_config->marcos_por_proceso) {
    format_debug_log("memoria_principal.c@tiene_marcos_reservados_libres", "Marcos libres: %d -- Proceso: %d", mem_swap_config->marcos_por_proceso - marcos_ocupados_por_proceso, pid);
    return true;
  }
  error_log("memoria_principal.c@tiene_marcos_reservados_libres", "No hay marcos reservados libres del proceso");
  return false;
}

uint32_t encontrar_marco_libre(uint32_t pid){

    for(int i=0; i < mem_ppal->cant_marcos; i++){

        if(array_marcos[i].estado == 0){

            return i;
        }
    }
    error_log("memoria_principal.c@encontrar_marco_libre","No se encontro marco libre. No se debería haber ejecutado esta función sin un checkeo previo....");
    return -1;
}


void iniciar_listas_globales_de_tablas() {
    debug_log("memoria_principal.c@iniciar_listas_globales_de_tablas", "Comienza el inicio la lista global de tablas de 1er nivel y la lista global de tablas de 2do nivel");
    lista_tablas_1er_nivel = list_create();
    lista_tablas_2do_nivel = list_create();
    debug_log("memoria_principal.c@iniciar_listas_globales_de_tablas", "Finaliza el inicio la lista global de tablas de 1er nivel y la lista global de tablas de 2do nivel");
}

void destruir_listas_globales_de_tablas(){
   debug_log("memoria_principal.c@destruir_listas_globales_de_tablas", "Se destruye la lista global de tablas de 1er nivel y la lista global de tablas de 2do nivel");
   list_destroy(lista_tablas_1er_nivel); 
   list_destroy_and_destroy_elements(lista_tablas_2do_nivel, (void*)free);
}


void iniciar_memoria(){
  iniciar_memoria_principal(); 
  iniciar_listas_globales_de_tablas(); 
  iniciar_swap();
  iniciar_lista_punteros_clock();
}


uint32_t* leer(uint32_t direccion_fisica) {
    uint32_t* contenido = malloc(sizeof (uint32_t));
    memcpy( contenido ,mem_ppal->memoria_principal + direccion_fisica, sizeof(uint32_t));
    return contenido;
}

void escribir(uint32_t direccion_fisica, uint32_t contenido){

    memcpy(mem_ppal->memoria_principal + direccion_fisica, &contenido, sizeof(uint32_t));
}



/*-------------------------SUSPENSION PROCESO --------------------------------*/

void* leer_pagina_memoria(uint32_t marco_asignado) {

  void *contenido_leido = malloc(sizeof(char) * mem_swap_config->tam_pagina);
  memcpy(contenido_leido, mem_ppal->memoria_principal + (marco_asignado * mem_swap_config->tam_pagina), mem_swap_config->tam_pagina);
 // format_debug_log("memoria_principal.c@leer_pagina_memoria",  "Marco: %d -- Contenido: [ -  %d - ]", marco_asignado, contenido_leido);
  return contenido_leido;
}


int escribir_contenido_swap(void* contenido_marco, uint32_t pid, uint32_t marco){

  archivo_pid_t* proceso = get_proceso_swap(pid);
  uint32_t offset = marco * mem_swap_config->tam_pagina;

  usleep(mem_swap_config->retardo_swap);

  memcpy(proceso->area_archivo_swap + offset, contenido_marco, mem_swap_config->tam_pagina);
  
  if( msync( proceso->area_archivo_swap, proceso->tam_proceso , MS_SYNC) < 0){
    format_error_log("memoria_principal.c@escribir_contenido_swap", "ERROR al escribir cambios en SWAP - Proceso: %d", pid);
    return 0; 
  }
  format_info_log("memoria_principal.c@escribir_contenido_swap", "EXITO al escribir cambios en SWAP - Proceso: %d", pid);
  return 1; 
} 


void bajar_paginas_swap(uint32_t pid) {

  for (int marco = 0; marco < mem_ppal->cant_marcos; marco++) {

    if (array_marcos[marco].pid == pid && array_marcos[marco].pagina->bit_modificado == 1 && array_marcos[marco].estado == 1 && array_marcos[marco].pagina->bit_presencia == 1) {     
      void *contenido_marco = leer_pagina_memoria(marco);
      int resultado = escribir_pagina_swap(pid, array_marcos[marco].pagina->nro_pagina); 

      if(resultado){

        pagina_2do_nivel_t *pagina_swapeada = array_marcos[marco].pagina;
        pagina_swapeada->bit_modificado = 0;
        pagina_swapeada->bit_presencia = 0;
        pagina_swapeada->bit_uso = 0;
        pagina_swapeada->marco = -1;
        array_marcos[marco].estado = 0; 
        array_marcos[marco].pid = -1; 
        free(contenido_marco);
        format_debug_log("memoria_suspender_proceso.c@bajar_paginas_swap", "Nro. entrada 2do nivel: %d - Viajo a swap", array_marcos[marco].pagina->nro_pagina);
      }
      else {
          // TODO Debería retornar en caso de error??
          format_debug_log("memoria_suspender_proceso.c@bajar_paginas_swap", "ERROR - No se puede bajar pagina a Swap", array_marcos[marco].pagina->nro_pagina);
      }
    }

  }
}

void imprimir_estado_array_MP() {

    int marco = 0;
    for (marco = 0; marco < mem_ppal->cant_marcos; marco++) {

        char *msg = string_from_format("Marco: %d  PID: %d BU: %i BM: %i BP: %i Estado: %d", marco, array_marcos[marco].pid, array_marcos[marco].pagina->bit_uso, array_marcos[marco].pagina->bit_modificado, array_marcos[marco].pagina->bit_presencia , array_marcos[marco].estado);
        warning_log("memoria_principal.c@imprimir_estado_array_MP", msg);
        free(msg);
    }
}





