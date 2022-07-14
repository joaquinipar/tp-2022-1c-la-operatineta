#!/bin/sh
ir_a_cpu() {
    cd "/home/utnso/Workspace/tp-2022-1c-la-operatineta/cpu/" || exit
}
ir_a_cpu
make
./bin/cpu.out ./arch_config/prueba_integral/integral.config
