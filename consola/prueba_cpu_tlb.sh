#!/bin/sh
echo "Corriendo 2 consolas con TLB_1, TLB_2"
bin/consola.out instrucciones/TLB_1.txt 2048 config/ejemplo.config & (sleep 0.1; bin/consola.out instrucciones/TLB_2.txt 2048 config/ejemplo.config ) 