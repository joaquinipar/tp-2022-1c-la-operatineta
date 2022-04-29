#ifndef MEMORIA_SWAP_MEMORIA_PRINCIPAL_HELPER_H
#define MEMORIA_SWAP_MEMORIA_PRINCIPAL_HELPER_H

#include <utils/logger.h>
#include <math.h>
#include "struct_mem.h"
#include "memoria_principal.h"
#include "struct_mem.h"


entrada_1er_nivel_t*  buscar_tabla_1er_nivel_de_un_proceso(int pid);
int proceso_presente_en_lista_tabla_1er_nivel(int pid);
entrada_1er_nivel_t* buscar_proceso_en_lista_tablas_2do_nivel(int pid);
entrada_1er_nivel_t* crear_tabla_1er_nivel_inicializada(uint32_t pid);
void inicializar_tabla_1er_nivel(entrada_1er_nivel_t* tabla_1er_nivel, uint32_t pid);
int agregar_entrada_tabla_1er_nivel(entrada_1er_nivel_t* tabla_1er_nivel, uint32_t nro_tabla_2do_nivel);
tabla_2do_nivel_t* crear_entradas_tabla_2do_nivel(uint32_t pid);
int32_t obtener_cant_paginas_a_utilizar(int32_t tamanio_total_size);

#endif //MEMORIA_SWAP_MEMORIA_PRINCIPAL_HELPER_H
