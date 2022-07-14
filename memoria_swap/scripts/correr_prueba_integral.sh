#!/bin/sh
ir_a_memoria() {
    cd "/home/utnso/Workspace/tp-2022-1c-la-operatineta/memoria_swap/" || exit
}
ir_a_memoria
make
./bin/memoria_swap.out ./arch_config/prueba_integral/integral.config

