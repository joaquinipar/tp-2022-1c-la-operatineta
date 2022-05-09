#include "../include/conexion_client.h"

int iniciar_conexion_memoria()
{
    debug_log("conexion_client.c@iniciar_conexion_memoria", "Se inicia conexion con Memoria");

    socket_cliente_kernel = crear_conexion(kernel_config->ip_memoria, kernel_config->puerto_memoria, "Memoria", "conexion_client.c@iniciar_conexion_memoria");

    if (socket_cliente_kernel == -1)
    {
        return 0;
    }
    return 1;
}


bool enviar_mensaje_valor_tabla_1er_nivel(uint32_t pid, uint32_t tamanio, uint32_t** valor_tabla_1er_nivel){
    
    debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Serializando y enviando mensaje VALUE_TAB_PAG");
    //CODOP + PID + TAMANIO
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = OPCODE_VALUE_TAB_PAG;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &tamanio, sizeof(uint32_t));
    
    
    int send_result = send(socket_cliente_kernel, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Comienza recepcion de mensaje - VALUE_TAB_PAG");
      uint32_t codigo_operacion;
      *valor_tabla_1er_nivel = malloc(sizeof(uint32_t));

        if (recv(socket_cliente_kernel, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
          format_debug_log(".c@enviar_mensaje_valor_tabla_1er_nivel", "Codigo de operacion: %d", codigo_operacion);
          info_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "El codOp no corresponde al protocolo de Comunicacion!");
        return false;
        }
        recv(socket_cliente_kernel, *valor_tabla_1er_nivel, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_valor_tabla_1er_nivel","Pid: %d - Valor de tabla de 1er Nivel :%d", pid, *valor_tabla_1er_nivel); 
        debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Termina mensaje ok - VALUE_TAB_PAG");
        return true; 
    }    

    error_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "[ERROR] Envio mensaje VALUE_TAB_PAG");
    debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Termina mensaje VALUE_TAB_PAG");
  return false;

}

