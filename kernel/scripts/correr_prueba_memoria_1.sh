#!/bin/sh
ir_a_kernel() {
    cd "/home/utnso/tp-2022-1c-la-operatineta/kernel/" || exit
}
ir_a_kernel
make
./bin/kernel.out ./arch_config/pruebas_memoria/kernel_memoria_1.config
