#!/bin/sh
ir_a_memoria() {
    cd "/home/utnso/tp-2022-1c-la-operatineta/memoria_swap/" || exit
}
ir_a_memoria
make
valgrind --leak-check=full ./bin/memoria_swap.out ./arch_config/prueba_tlb_fifo_clkmod.config

