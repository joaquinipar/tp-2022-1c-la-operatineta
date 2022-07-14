#!/bin/sh
ir_a_consola() {
    cd "/home/utnso/tp-2022-1c-la-operatineta/consola/" || exit
}
ir_a_consola
echo "Corriendo prueba integral 3"
./bin/consola.out ./instrucciones/INTEGRAL_3 2048 ./config/ejemplo.config
