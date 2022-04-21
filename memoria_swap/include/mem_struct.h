#ifndef INCLUDE_MEM_STRUCT_H_
#define INCLUDE_MEM_STRUCT_H_

#include <stdlib.h>
#include <stdint.h> 
#include <utils/logger.h>

typedef struct {
    uint32_t nro_pagina; 
    uint32_t marco; 
    uint32_t bit_presencia; 
    uint32_t bit_modificado;
    uint32_t bit_uso;  
    long long time_ref;  
} pagina_2do_nivel_t; 

typedef struct {
    pagina_2do_nivel_t* contenido_tabla_2do_nivel;
} tabla_2do_nivel_t; 

typedef struct {
    uint32_t pid;
    int* tabla_pagina_1er_nivel; //array guarda el numero de tabla de 2do
    tabla_2do_nivel_t* tabla_pagina_2do_nivel;
} tabla_t;

//Estructuras Memoria 
typedef struct {
    uint32_t pid;
    pagina_2do_nivel_t* pagina; 
    int estado;     
}marco_t;  

typedef struct{
    void* memoria_principal; 
    int   cant_marcos; 
}mem_principal_t; 
//Estructuras Memoria 

mem_principal_t* mem_ppal; // Struct de la void* de memoria 
marco_t*  array_marcos;  // Array de cantidad de marcos. Indice es el marco. Tipo marco_t*

#endif /*INCLUDE_MEM_STRUCT_H_*/