#!/bin/sh
ir_a_kernel() {
    cd "/home/utnso/tp-2022-1c-la-operatineta/kernel/" || exit
}
ir_a_kernel
make
valgrind --leak-check=full ./bin/kernel.out ./arch_config/prueba_kernel_tlb.config
