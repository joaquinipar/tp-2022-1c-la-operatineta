#!/bin/sh
echo "Corriendo 3 consolas con BASE_1, BASE_2, BASE_2"
bin/consola.out instrucciones/BASE_1.txt 512 config/ejemplo.config & (sleep 0.1; bin/consola.out instrucciones/BASE_2.txt 512 config/ejemplo.config ) & (sleep 0.2;bin/consola.out instrucciones/BASE_2.txt 512 config/ejemplo.config)