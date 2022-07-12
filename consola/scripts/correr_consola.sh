#!/bin/sh
ir_a_consola() {
    cd "/home/utnso/tp-2022-1c-la-operatineta/consola/" || exit
}
ir_a_consola
make
./bin/consola.out ./config/ejemplo.config
