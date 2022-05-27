#ifndef MEMORIA_SWAP_ALGORITMOS_REEMPLAZO_H
#define MEMORIA_SWAP_ALGORITMOS_REEMPLAZO_H

#include <stdint.h>
#include <utils/logger.h>
#include <commons/collections/list.h>
#include "../configuracion_mem_swap.h"
#include "../struct_mem.h"


typedef struct {
    uint32_t pid;
    uint32_t marco_apuntado;
} puntero_clock_modificado;

t_list* punteros_procesos;


uint32_t correr_algoritmo_reemplazo(uint32_t pid);
uint32_t manejar_clock(uint32_t pid);
uint32_t manejar_clock_modificado(uint32_t pid);

void iniciar_lista_punteros_clock_modificado_fija();
int agregar_puntero_nuevo_clock(uint32_t pid);
int obtener_puntero_clock(uint32_t pid);
void destruir_lista_de_punteros_clock();
int setear_marco_a_puntero_clock(uint32_t pid, uint32_t marco_a_setear);
void mover_puntero_fija(int marco, uint32_t pid);
uint32_t get_proximo_marco_del_proceso(uint32_t pid, uint32_t marco_actual);
uint32_t get_primer_marco_allocado(uint32_t pid);
uint32_t get_marco_reservado_por_proceso(uint32_t pid);
int _struct_esta_corrupto(uint32_t bit_uso, uint32_t bit_modificado);

#endif //MEMORIA_SWAP_ALGORITMOS_REEMPLAZO_H
