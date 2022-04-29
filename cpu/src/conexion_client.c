#include "../include/conexion_client.h"

int iniciar_conexion_memoria()
{
    debug_log("conexion_client.c@iniciar_conexion_memoria", "Se inicia conexion con Memoria");

    socket_cliente_cpu = crear_conexion(cpu_config->ip_memoria, cpu_config->puerto_memoria, "Memoria", "conexion_client.c@iniciar_conexion_memoria");

    if (socket_cliente_cpu == -1)
    {
        return 0;
    }
    return 1;
}

bool enviar_mensaje_acceso_1er_nivel(uint32_t pid, uint32_t entrada_1er_nivel, uint32_t nro_tabla_1er_nivel, uint32_t** nro_tabla_2do_nivel)
{

    debug_log(".c@enviar_mensaje_acceso_1er_nivel", "Serializando y enviando mensaje ACCESO_1ER_NIVEL");
    //CODOP + PID + ENTRADA 1ER NIVEL + NRO TABLA 1ER NIVEL
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = ACCESO_1ER_NIVEL;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &entrada_1er_nivel, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t )+ sizeof(uint32_t), &nro_tabla_1er_nivel, sizeof(uint32_t));

    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      debug_log(".c@enviar_mensaje_acceso_1er_nivel", "Comienza recepcion de mensaje - ACCESO_1ER_NIVEL");
      uint32_t codigo_operacion;
      uint32_t pid;
      *nro_tabla_2do_nivel = malloc(sizeof(uint32_t));

        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
          format_debug_log(".c@enviar_mensaje_acceso_1er_nivel", "Codigo de operacion: %d", codigo_operacion);
          info_log(".c@enviar_mensaje_acceso_1er_nivel", "El codOp no corresponde al protocolo de Comunicacion!");
        return false;
        }
        recv(socket_cliente_cpu, &pid, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_acceso_1er_nivel","Pid:%d", pid); 
        recv(socket_cliente_cpu, *nro_tabla_2do_nivel, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_acceso_1er_nivel","Nro de tabla de 2do nivel:%d", *nro_tabla_2do_nivel); 
        debug_log(".c@enviar_mensaje_acceso_1er_nivel", "Termina mensaje ok - ACCESO_1ER_NIVEL");
        return true; 
    }    

    error_log(".c@enviar_mensaje_acceso_1er_nivel", "[ERROR] Envio mensaje ACCESO_1ER_NIVEL");
    debug_log(".c@enviar_mensaje_acceso_1er_nivel", "Termina mensaje ACCESO_1ER_NIVEL");
  return false;
}

bool enviar_mensaje_acceso_2do_nivel(uint32_t pid, uint32_t nro_tabla_2do_nivel, uint32_t nro_pagina, uint32_t** nro_marco)
{
    debug_log(".c@enviar_mensaje_acceso_2do_nivel", "Serializando y enviando mensaje ACCESO_2DO_NIVEL");
    //CODOP + PID + NRO TABLA 2DO NIVEL + NRO PAGINA
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = ACCESO_2DO_NIVEL;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &nro_tabla_2do_nivel, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t ) + sizeof(uint32_t), &nro_pagina, sizeof(uint32_t));

    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      debug_log(".c@enviar_mensaje_acceso_2do_nivel", "Comienza recepcion de mensaje - ACCESO_2DO_NIVEL");
      uint32_t codigo_operacion; 
      *nro_marco =malloc(sizeof(uint32_t));      

        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
        format_debug_log(".c@enviar_mensaje_acceso_2do_nivel", "Codigo de operacion: %d", codigo_operacion);
        info_log(".c@enviar_mensaje_acceso_2do_nivel", "El codOp no corresponde al protocolo de Comunicacion!");
        return false;
        }
        recv(socket_cliente_cpu, &pid, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_acceso_2do_nivel","Pid:%d", pid); 
        recv(socket_cliente_cpu, *nro_marco, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_acceso_2do_nivel","Nro de marco:%d", *nro_marco); 
        debug_log(".c@enviar_mensaje_acceso_2do_nivel", "Termina mensaje ok - ACCESO_2DO_NIVEL");
        return true; 
    }    
    
    error_log(".c@enviar_mensaje_acceso_2do_nivel", "[ERROR] Envio mensaje ACCESO_2DO_NIVEL");
    debug_log(".c@enviar_mensaje_acceso_2do_nivel", "Termina mensaje ACCESO_2DO_NIVEL");
  return false;
}

bool enviar_mensaje_read(uint32_t pid, uint32_t direccion_fisica, uint32_t** valor_a_imprimir)
{

    debug_log(".c@enviar_mensaje_read", "Serializando y enviando mensaje READ");
    //CODOP + PID + DIRECCION FISICA
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = READ;
    

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &direccion_fisica, sizeof(uint32_t));
    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      debug_log(".c@enviar_mensaje_read", "Comienza recepcion de mensaje - READ");
      uint32_t codigo_operacion;
      uint32_t pid;
      *valor_a_imprimir =malloc(sizeof(uint32_t));
      
        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
        format_debug_log(".c@enviar_mensaje_read", "Codigo de operacion: %d", codigo_operacion);
        info_log(".c@enviar_mensaje_read", "El codOp no corresponde al protocolo de Comunicacion!");
        return false;
        }
        recv(socket_cliente_cpu, &pid, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_read","Pid:%d", pid); 
        recv(socket_cliente_cpu, *valor_a_imprimir, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_read","Valor a imprimir:%d", *valor_a_imprimir); 
        debug_log(".c@enviar_mensaje_read", "Termina mensaje ok - READ");
        return true; 
    }    
    
    error_log(".c@enviar_mensaje_read", "[ERROR] Envio mensaje READ");
    debug_log(".c@enviar_mensaje_read", "Termina mensaje READ");
  return false;
}

bool enviar_mensaje_write(uint32_t pid, uint32_t direccion_fisica, uint32_t valor_a_escribir)
{
    debug_log(".c@enviar_mensaje_write", "Serializando y enviando mensaje WRITE");
    //CODOP + PID + DIRECCION FISICA + VALOR A ESCRIBIR
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = WRITE;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &direccion_fisica, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t), &valor_a_escribir, sizeof(uint32_t));

    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      debug_log(".c@enviar_mensaje_write", "Comienza recepcion de mensaje - WRITE");
      bool response = recv_ack(socket_cliente_cpu);
      if (response) {
      debug_log(".c@enviar_mensaje_write", "Recepcion mensaje ACK OK - WRITE");
      debug_log(".c@enviar_mensaje_write", "Termina mensaje WRITE");
      return true; 
      }
      error_log("protocolo_comunicacion_mem.c@enviar_mensaje_destruir_pagina", "Recepcion mensaje ACK ERROR -WRITE");
      debug_log(".c@enviar_mensaje_write", "Termina mensaje ok - WRITE");
      return false; 
    }    
    
    error_log(".c@enviar_mensaje_write", "[ERROR] Envio mensaje WRITE");
    return false;
}

bool enviar_mensaje_inicial_configuracion(){
  info_log(".c@enviar_mensaje_inicial_configuracion", "Solicitud de configuracion relevamente para MMU - Mensaje PING_PONG_MEMORIA");

    int stream_size = sizeof(op_code_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = PING_PONG_MEMORIA;
    memcpy(stream, &op_code, sizeof(op_code_t));
  
    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

      if (send_result != -1) {
      debug_log(".c@enviar_mensaje_read", "Comienza recepcion de mensaje - PING_PONG_MEMORIA");
      uint32_t codigo_operacion;
      mensaje_configuracion_t* datos_traduccion = malloc(sizeof(mensaje_configuracion_t));

        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
        format_debug_log(".c@enviar_mensaje_inicial_configuracion", "Codigo de operacion: %d", codigo_operacion);
        info_log(".c@enviar_mensaje_inicial_configuracion", "El codOp no corresponde al protocolo de Comunicacion!");
        return false;
        }
        recv(socket_cliente_cpu, &datos_traduccion->cantidad_entradas_tabla, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_inicial_configuracion","Cantidad de entradas de tabla:%d", datos_traduccion->cantidad_entradas_tabla); 
        recv(socket_cliente_cpu, &datos_traduccion->tamanio_pagina, sizeof(uint32_t), false);
        format_debug_log(".c@enviar_mensaje_inicial_configuracion","Tamaño de pagina:%d", datos_traduccion->tamanio_pagina); 
        debug_log(".c@enviar_mensaje_inicial_configuracion", "Termina mensaje ok - PING_PONG_MEMORIA");
        return true; 
    }   
    error_log(".c@enviar_mensaje_read", "[ERROR] Envio mensaje PING_PONG_MEMORIA");
    debug_log(".c@enviar_mensaje_read", "Termina mensaje PING_PONG_MEMORIA");
  return false;

}