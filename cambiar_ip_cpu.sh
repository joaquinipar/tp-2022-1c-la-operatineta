#!/bin/sh
read -p "Ingrese nueva IP_CPU: " IP_CPU

echo "Cambiando IP_CPU en kernel a $IP_CPU"
find ./kernel/arch_config/ -type f -exec sed -i "s/IP_CPU=.*/IP_CPU=$IP_CPU/g" {} +

echo "Cambiando IP_CPU en memoria_swap a $IP_CPU"
find ./memoria_swap/arch_config/ -type f -exec sed -i "s/IP_CPU=.*/IP_CPU=$IP_CPU/g" {} +

echo "Cambiando IP_CPU en cpu a $IP_CPU"
find ./cpu/arch_config/ -type f -exec sed -i "s/IP_CPU=.*/IP_CPU=$IP_CPU/g" {} +

echo "Listo!"