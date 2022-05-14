#ifndef MEMORIA_SWAP_INCLUDE_STRUCT_MEM_H_
#define MEMORIA_SWAP_INCLUDE_STRUCT_MEM_H_

#include <stdlib.h>
#include <stdint.h> 
#include <utils/logger.h>
#include <commons/collections/list.h>

typedef struct {
    uint32_t nro_pagina; 
    uint32_t marco; 
    uint32_t bit_presencia; 
    uint32_t bit_modificado;
    uint32_t bit_uso;  
    long long time_ref;  
} pagina_2do_nivel_t; 

typedef struct {
    uint32_t pid;
    pagina_2do_nivel_t* contenido_tabla_2do_nivel;
} tabla_2do_nivel_t;

/*
 * La entrada de 1er nivel es una entrada de la tabla de paginas de primer nivel.
 * Un array de entrada_1er_nivel_t representa una tabla de primer nivel.
 */
typedef struct {
    uint32_t pid;
    uint32_t nro_tabla_2do_nivel;
} entrada_1er_nivel_t;


//Estructuras Memoria 
typedef struct {
    uint32_t pid; // == -1 -> Sin asignar.
    pagina_2do_nivel_t* pagina;
    int estado; // == 1 -> Ocupado. == 0 -> Libre
}marco_t;  

typedef struct{
    void* memoria_principal; 
    int   cant_marcos; 
}mem_principal_t;  
//Estructuras Memoria 

mem_principal_t* mem_ppal; // Struct de la void* de memoria 
marco_t*  array_marcos;  // Array de cantidad de marcos. Indice es el marco. Tipo marco_t*


#endif /* MEMORIA_SWAP_INCLUDE_STRUCT_MEM_H_ */
