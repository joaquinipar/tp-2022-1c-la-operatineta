#!/bin/sh
ir_a_kernel() {
    cd "/home/utnso/Workspace/tp-2022-1c-la-operatineta/kernel/" || exit
}
ir_a_kernel
make
./bin/kernel.out ./arch_config/prueba_integral/integral.config
