#ifndef __UTILS_STRUCTS_H
#define __UTILS_STRUCTS_H

#include <stdint.h>
#include <commons/collections/list.h>
#include <sys/time.h>

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

typedef enum estado_proceso_t {
  ESTADO_PROCESO_NEW,
  ESTADO_PROCESO_READY,
  ESTADO_PROCESO_EXEC,
  ESTADO_PROCESO_BLOCKED,
  ESTADO_PROCESO_BLOCKED_SUSPENDED,
  ESTADO_PROCESO_SUSPENDED_READY,
  ESTADO_PROCESO_EXIT
} estado_proceso_t;

typedef struct rango_tiempo_t {
  struct timeval inicio;
  struct timeval fin;
} rango_tiempo_t;

typedef struct {

  uint32_t pid;
  t_list*  lista_instrucciones;
  uint32_t tamanio;
  uint32_t program_counter;
  uint32_t tabla_paginas;
  uint32_t estimacion_rafaga;
  estado_proceso_t estado; 
  uint32_t socket;

  // Calculo de estimado: al desalojar el proceso se debe pisar la estimacion
  // estimacion = estimado * alfa + duracion_ultima_rafaga * (1 - alfa)
  // cuando se conecta un proceso por consola, estimacion es el valor en la config
  // duracion de la ultima rafaga es 0
  uint32_t estimacion;
  uint32_t duracion_ultima_rafaga;

  /* contiene la rafaga del proceso en ejecucion,
      inicio: momento en que se inicio la rafaga
      fin: se va a actualizar cuando el proceso sale del cpu para realizar el calculo correspondiente y luego poner ambos 0
  */
  rango_tiempo_t rafaga_actual;

} pcb_t;

#endif