#!/bin/sh
echo "Corriendo 3 consolas con PLANI_1, PLANI_2, PLANI_2"
bin/consola.out instrucciones/PLANI_1.txt 512 config/ejemplo.config & (sleep 0.1; bin/consola.out instrucciones/PLANI_2.txt 512 config/ejemplo.config ) & (sleep 0.2;bin/consola.out instrucciones/PLANI_2.txt 512 config/ejemplo.config)