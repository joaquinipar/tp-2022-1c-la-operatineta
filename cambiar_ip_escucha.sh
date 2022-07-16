#!/bin/sh
read -p "Ingrese nueva IP_ESCUCHA: " IP_ESCUCHA

echo "Cambiando IP_ESCUCHA en kernel a $IP_ESCUCHA"
find ./kernel/arch_config/ -type f -exec sed -i "s/IP_ESCUCHA=.*/IP_ESCUCHA=$IP_ESCUCHA/g" {} +

echo "Cambiando IP_ESCUCHA en memoria_swap a $IP_ESCUCHA"
find ./memoria_swap/arch_config/ -type f -exec sed -i "s/IP_ESCUCHA=.*/IP_ESCUCHA=$IP_ESCUCHA/g" {} +

echo "Cambiando IP_ESCUCHA en cpu a $IP_ESCUCHA"
find ./cpu/arch_config/ -type f -exec sed -i "s/IP_ESCUCHA=.*/IP_ESCUCHA=$IP_ESCUCHA/g" {} +

echo "Listo!"