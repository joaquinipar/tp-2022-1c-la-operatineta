#!/bin/sh
ir_a_cpu() {
    cd "/home/utnso/tp-2022-1c-la-operatineta/cpu/" || exit
}
ir_a_cpu
make
valgrind ./bin/cpu.out ./arch_config/prueba_tlb_fifo.config
