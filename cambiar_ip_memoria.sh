#!/bin/sh
read -p "Ingrese nueva IP_MEMORIA: " IP_MEMORIA

echo "Cambiando IP_MEMORIA en kernel a $IP_MEMORIA"
find ./kernel/arch_config/ -type f -exec sed -i "s/IP_MEMORIA=.*/IP_MEMORIA=$IP_MEMORIA/g" {} +

echo "Cambiando IP_MEMORIA en memoria_swap a $IP_MEMORIA"
find ./memoria_swap/arch_config/ -type f -exec sed -i "s/IP_MEMORIA=.*/IP_MEMORIA=$IP_MEMORIA/g" {} +

echo "Cambiando IP_MEMORIA en cpu a $IP_MEMORIA"
find ./cpu/arch_config/ -type f -exec sed -i "s/IP_MEMORIA=.*/IP_MEMORIA=$IP_MEMORIA/g" {} +

echo "Listo!"