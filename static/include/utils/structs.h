#ifndef __UTILS_STRUCTS_H
#define __UTILS_STRUCTS_H

#include <stdint.h>

typedef enum{

    NO_OP = 0,
    IO = 1,
    READ = 2,
    WRITE = 3,
    COPY = 4,
    EXIT = 5

} instruccion_code_t;

typedef struct {

    instruccion_code_t instruccion;
    int32_t argumentos[2];

} instruccion_t;

#endif