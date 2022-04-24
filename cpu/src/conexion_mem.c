#include "../include/conexion_mem.h"

int comenzar_conexion_memoria(){

   int socket_cliente = crear_conexion(cpu_config->ip_memoria, cpu_config->puerto_memoria, "Mem", "conexion_mem.c@comenzar_conexion_memoria");
   return socket_cliente;
}

int iniciar_conexion_mem() {
    info_log("conexion_mem.c@iniciar_conexion_mem", "Se inicia conexion con el Servidor de Memoria");
   
    socket_cliente_cpu = comenzar_conexion_memoria();

    if(socket_cliente_cpu == -1){
       return 0; 
    }
    return 1; 

}
