#!/bin/sh
read -p "Ingrese nueva IP_KERNEL: " IP_KERNEL

echo "Cambiando IP_KERNEL en consola a $IP_KERNEL"
find ./consola/config/ -type f -exec sed -i "s/IP_KERNEL=.*/IP_KERNEL=$IP_KERNEL/g" {} +

echo "Cambiando IP_KERNEL en kernel a $IP_KERNEL"
find ./kernel/arch_config/ -type f -exec sed -i "s/IP_KERNEL=.*/IP_KERNEL=$IP_KERNEL/g" {} +

echo "Cambiando IP_KERNEL en memoria_swap a $IP_KERNEL"
find ./memoria_swap/arch_config/ -type f -exec sed -i "s/IP_KERNEL=.*/IP_KERNEL=$IP_KERNEL/g" {} +

echo "Cambiando IP_KERNEL en cpu a $IP_KERNEL"
find ./cpu/arch_config/ -type f -exec sed -i "s/IP_KERNEL=.*/IP_KERNEL=$IP_KERNEL/g" {} +

echo "Listo!"