#!/bin/sh
echo "Corriendo 3 consolas con SUSPE_1, SUSPE_2, SUSPE_3"
bin/consola.out instrucciones/SUSPE_1.txt 512 config/ejemplo.config & (sleep 0.1; bin/consola.out instrucciones/SUSPE_2.txt 512 config/ejemplo.config ) & (sleep 0.2;bin/consola.out instrucciones/SUSPE_3.txt 512 config/ejemplo.config)