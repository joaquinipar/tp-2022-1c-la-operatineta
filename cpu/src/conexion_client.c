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

uint32_t enviar_mensaje_acceso_1er_nivel(uint32_t pid, uint32_t entrada_1er_nivel, uint32_t nro_tabla_1er_nivel)
{
    info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "-----------------------------------------------------------");
    info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "Serializando y enviando mensaje OPCODE_ACCESO_1ER_NIVEL");
    format_info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel","Envio PID: %i Entrada_1er_nivel: %i Nro_tabla_1er_nivel: %i", pid, entrada_1er_nivel, nro_tabla_1er_nivel);
    //CODOP + PID + ENTRADA 1ER NIVEL + NRO TABLA 1ER NIVEL
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = OPCODE_ACCESO_1ER_NIVEL;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &nro_tabla_1er_nivel, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t), &entrada_1er_nivel, sizeof(uint32_t));


    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "Comienza recepcion de mensaje - OPCODE_ACCESO_1ER_NIVEL");
      uint32_t codigo_operacion;
      uint32_t pid;
      uint32_t nro_tabla_2do_nivel; 
        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
          format_debug_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "Codigo de operacion: %d", codigo_operacion);
          info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "El codOp no corresponde al protocolo de Comunicacion!");
          info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "-----------------------------------------------------------");
        return -1;
        }
        recv(socket_cliente_cpu, &pid, sizeof(uint32_t), false);
       // format_debug_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel","Pid:%d", pid); 
        recv(socket_cliente_cpu, &nro_tabla_2do_nivel, sizeof(uint32_t), false);
        format_info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel","PID: %d - Nro de tabla de 2do nivel: %d", pid, nro_tabla_2do_nivel); 
        info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "Termina mensaje ok - OPCODE_ACCESO_1ER_NIVEL");
        info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "-----------------------------------------------------------");
        return nro_tabla_2do_nivel; 
    }    

    error_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "[ERROR] Envio mensaje OPCODE_ACCESO_1ER_NIVEL");
    info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "Termina mensaje con error - OPCODE_ACCESO_1ER_NIVEL");
    info_log("conexion_client.c@enviar_mensaje_acceso_1er_nivel", "-----------------------------------------------------------");
  return -1;
}

uint32_t enviar_mensaje_acceso_2do_nivel(uint32_t pid, uint32_t nro_tabla_2do_nivel, uint32_t entrada_2do_nivel)
{
    info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "-----------------------------------------------------------");
    info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "Serializando y enviando mensaje OPCODE_ACCESO_2DO_NIVEL");
    //CODOP + PID + NRO TABLA 2DO NIVEL + NRO PAGINA
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = OPCODE_ACCESO_2DO_NIVEL;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &nro_tabla_2do_nivel, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t ) + sizeof(uint32_t), &entrada_2do_nivel, sizeof(uint32_t));

    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "Comienza recepcion de mensaje - OPCODE_ACCESO_2DO_NIVEL");
      uint32_t codigo_operacion; 
      uint32_t nro_marco;    

        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
        format_debug_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "Codigo de operacion: %d", codigo_operacion);
        info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "El codOp no corresponde al protocolo de Comunicacion!");
        info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "-----------------------------------------------------------");
        return -1;
        }
        recv(socket_cliente_cpu, &pid, sizeof(uint32_t), false);
        format_debug_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel","Pid:%d", pid); 
        recv(socket_cliente_cpu, &nro_marco, sizeof(uint32_t), false);
        format_info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel","PID: %d - Nro de marco: %d", pid, nro_marco); 
        info_log(".c@enviar_mensaje_acceso_2do_nivel", "Termina mensaje ok - OPCODE_ACCESO_2DO_NIVEL");
        info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "-----------------------------------------------------------");
        return nro_marco; 
    }    
    
    error_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "[ERROR] Envio mensaje OPCODE_ACCESO_2DO_NIVEL");
    info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "Termina mensaje con error - OPCODE_ACCESO_2DO_NIVEL");
    info_log("conexion_client.c@enviar_mensaje_acceso_2do_nivel", "-----------------------------------------------------------");
  return -1;
}

uint32_t enviar_mensaje_read(uint32_t pid, uint32_t direccion_fisica)
{
    info_log("conexion_client.c@enviar_mensaje_read", "-----------------------------------------------------------");
    info_log("conexion_client.c@enviar_mensaje_read", "Serializando y enviando mensaje OPCODE_READ");
    //CODOP + PID + DIRECCION FISICA
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = OPCODE_READ;
    

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &direccion_fisica, sizeof(uint32_t));
    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      info_log("conexion_client.c@enviar_mensaje_read", "Comienza recepcion de mensaje - OPCODE_READ");
      uint32_t codigo_operacion;
      uint32_t pid;
      uint32_t valor_a_imprimir; 

        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
        format_debug_log("conexion_client.c@enviar_mensaje_read", "Codigo de operacion: %d", codigo_operacion);
        info_log("conexion_client.c@enviar_mensaje_read", "El codOp no corresponde al protocolo de Comunicacion!");
        info_log("conexion_client.c@enviar_mensaje_read", "-----------------------------------------------------------");
        return -1;
        }
        recv(socket_cliente_cpu, &pid, sizeof(uint32_t), false);
        format_debug_log("conexion_client.c@enviar_mensaje_read","Pid:%d", pid); 
        recv(socket_cliente_cpu, &valor_a_imprimir, sizeof(uint32_t), false);
        format_debug_log("conexion_client.c@enviar_mensaje_read","Valor a imprimir:%d", valor_a_imprimir); 
        info_log("conexion_client.c@enviar_mensaje_read", "Termina mensaje ok - OPCODE_READ");
        info_log("conexion_client.c@enviar_mensaje_read", "-----------------------------------------------------------");
        return valor_a_imprimir; 
    }    
    
    error_log("conexion_client.c@enviar_mensaje_read", "[ERROR] Envio mensaje OPCODE_READ");
    info_log("conexion_client.c@enviar_mensaje_read", "Termina mensaje con error - OPCODE_READ");
    info_log("conexion_client.c@enviar_mensaje_read", "-----------------------------------------------------------");
  return -1;
}

uint32_t enviar_mensaje_write(uint32_t pid, uint32_t direccion_fisica, uint32_t valor_a_escribir)
{
    info_log("conexion_client.c@enviar_mensaje_write", "-----------------------------------------------------------");
    info_log("conexion_client.c@enviar_mensaje_write", "Serializando y enviando mensaje OPCODE_WRITE");
    //CODOP + PID + DIRECCION FISICA + VALOR A ESCRIBIR
    int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = OPCODE_WRITE;

    memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t), &direccion_fisica, sizeof(uint32_t));
    memcpy(stream + sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t), &valor_a_escribir, sizeof(uint32_t));

    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

    if (send_result != -1) {
      info_log("conexion_client.c@enviar_mensaje_write", "Comienza recepcion de mensaje - OPCODE_WRITE");
      bool response = recv_ack(socket_cliente_cpu);
      if (response) {
      info_log("conexion_client.c@enviar_mensaje_write", "Recepcion mensaje ACK OK - OPCODE_WRITE");
      info_log("conexion_client.c@enviar_mensaje_write", "Termina mensaje OPCODE_WRITE");
      info_log("conexion_client.c@enviar_mensaje_write", "-----------------------------------------------------------");
      return 1; 
      }
      error_log("protocolo_comunicacion_memconexion_client.c@enviar_mensaje_destruir_pagina", "Recepcion mensaje ACK ERROR -OPCODE_WRITE");
      info_log("conexion_client.c@enviar_mensaje_write", "Termina mensaje ack error- OPCODE_WRITE");
      info_log("conexion_client.c@enviar_mensaje_write", "-----------------------------------------------------------");
      return -1; 
    }    
    
    error_log("conexion_client.c@enviar_mensaje_write", "[ERROR] Envio mensaje OPCODE_WRITE");
    info_log("conexion_client.c@enviar_mensaje_write", "Termina mensaje ack error- OPCODE_WRITE");
    info_log("conexion_client.c@enviar_mensaje_write", "-----------------------------------------------------------");
    return -1;
  }

bool enviar_mensaje_inicial_configuracion(){
    info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "-----------------------------------------------------------");
    info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "Enviando handshake inicial a Memoria");
    int stream_size = sizeof(op_code_t);
    void *stream = malloc(stream_size);
    op_code_t op_code = OPCODE_PING_PONG_MEMORIA;
    memcpy(stream, &op_code, sizeof(op_code_t));
  
    int send_result = send(socket_cliente_cpu, stream, stream_size, false);
    free(stream); 

      if (send_result != -1) {
      
      info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "Comienza recepcion de mensaje - OPCODE_PING_PONG_MEMORIA");
      uint32_t codigo_operacion;
      datos_traduccion = malloc(sizeof(mensaje_configuracion_t));

        if (recv(socket_cliente_cpu, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
        format_debug_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "Codigo de operacion: %d", codigo_operacion);
        info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "El codOp no corresponde al protocolo de Comunicacion!");
        info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "-----------------------------------------------------------");
        return false;
        }


        recv(socket_cliente_cpu, &datos_traduccion->cantidad_entradas_tabla, sizeof(uint32_t), false);
        format_info_log("conexion_client.c@enviar_mensaje_inicial_configuracion","Cantidad de entradas de tabla:%d", datos_traduccion->cantidad_entradas_tabla); 
        recv(socket_cliente_cpu, &datos_traduccion->tamanio_pagina, sizeof(uint32_t), false);
        format_info_log("conexion_client.c@enviar_mensaje_inicial_configuracion","Tama??o de pagina:%d", datos_traduccion->tamanio_pagina); 
        info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "Termina mensaje ok - OPCODE_PING_PONG_MEMORIA");
        info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "-----------------------------------------------------------");
        return true; 
    }   
    error_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "[ERROR] Envio mensaje OPCODE_PING_PONG_MEMORIA");
    info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "Termina mensaje con error - OPCODE_PING_PONG_MEMORIA");
    info_log("conexion_client.c@enviar_mensaje_inicial_configuracion", "-----------------------------------------------------------");
  return false;
}


//OPCODE_PROCESO_DESALOJADO_IO
bool enviar_mensaje_proceso_desalojado_io(pcb_t* proceso_actualizado, int socket, uint32_t tiempo_bloqueo){
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "-----------------------------------------------------------");
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Comienza envio de mensaje - OPCODE_PROCESO_DESALOJADO_IO");
   // uint32_t stream_size_parcial = serializar_proceso(proceso_actualizado, OPCODE_PROCESO_DESALOJADO_IO, &stream);
  uint32_t tam_stream_list_instruc = tamanio_stream_lista_instrucciones(proceso_actualizado->lista_instrucciones);

	/*COD OP  +PID+ TAMANIO+PC +  TP + ESTADO+ ESTIMACIONRAFAGA+ ESTIMACION+DURACION+RAFAGAACTUAL + cant de instru + c/instruccion* 3 parametros*/
	uint32_t stream_size = sizeof(op_code_t) + sizeof(uint32_t) +sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tam_stream_list_instruc ;

	void *stream = malloc(stream_size);

	// format_debug_log("serializacion.c@serializar_proceso", "Stream List Instrucciones %d", tam_stream_list_instruc);
	format_debug_log("serializacion.c@serializar_proceso", "Stream Size %d", stream_size);
  op_code_t codigo_operacion = OPCODE_PROCESO_DESALOJADO_IO; 
	int desplazamiento = 0;
	memcpy(stream, &codigo_operacion, sizeof(op_code_t));
	desplazamiento += sizeof(op_code_t);
  memcpy(stream + desplazamiento, &tiempo_bloqueo, sizeof(uint32_t));
  desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->pid), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->tamanio), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->program_counter), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->tabla_paginas), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->estado), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->estimacion_rafaga), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->estimacion), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->duracion_ultima_rafaga), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(stream + desplazamiento, &(proceso_actualizado->rafaga_actual), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	serializar_lista_de_instrucciones(proceso_actualizado->lista_instrucciones, desplazamiento, stream);

    int send_result = send(socket, stream, stream_size , false);

    //Liberar memoria
    free(stream);
    //destruir_proceso(proceso_actualizado);


    format_debug_log("conexion_clien.c@enviar_mensaje_proceso_desalojado_io", "Send Result %d", send_result); 
    if (send_result != -1) {
    bool response = recv_ack(socket);
    if (response) {
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Recepcion mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_IO");
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Termina mensaje OPCODE_PROCESO_DESALOJADO_IO");
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "-----------------------------------------------------------");
      return response;
    }
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Recepcion mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_IO");
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Termina mensaje con error OPCODE_PROCESO_DESALOJADO_IO");
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "-----------------------------------------------------------");
    return response;
  }
  error_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Error al enviar mensaje OPCODE_PROCESO_DESALOJADO_IO");
  debug_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "Termina mensaje OPCODE_PROCESO_DESALOJADO_IO");
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_io", "-----------------------------------------------------------");
  return false;
}
//OPCODE_PROCESO_DESALOJADO_INTERRUPT

bool enviar_mensaje_proceso_desalojado_interrupt(pcb_t* proceso_actualizado, int socket){
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "-----------------------------------------------------------");  
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Comienza envio de mensaje - OPCODE_PROCESO_DESALOJADO_INTERRUPT");
    /*void *stream;
    uint32_t stream_size = serializar_proceso(proceso_actualizado,OPCODE_PROCESO_DESALOJADO_INTERRUPT, &stream);*/
    uint32_t tam_stream_list_instruc = tamanio_stream_lista_instrucciones(proceso_actualizado->lista_instrucciones);

    /*COD OP  +PID+ TAMANIO+PC +  TP + ESTADO+ ESTIMACIONRAFAGA+ ESTIMACION+DURACION+RAFAGAACTUAL + cant de instru + c/instruccion* 3 parametros*/
    uint32_t stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tam_stream_list_instruc ;

    void *stream = malloc(stream_size);

    // format_debug_log("serializacion.c@serializar_proceso", "Stream List Instrucciones %d", tam_stream_list_instruc);
    format_debug_log("serializacion.c@serializar_proceso", "Stream Size %d", stream_size);
    op_code_t codigo_operacion = OPCODE_PROCESO_DESALOJADO_INTERRUPT; 
    int desplazamiento = 0;
    memcpy(stream, &codigo_operacion, sizeof(op_code_t));
    desplazamiento += sizeof(op_code_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->pid), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->tamanio), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->program_counter), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->tabla_paginas), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->estado), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->estimacion_rafaga), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->estimacion), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->duracion_ultima_rafaga), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    memcpy(stream + desplazamiento, &(proceso_actualizado->rafaga_actual), sizeof(uint32_t));
    desplazamiento += sizeof(uint32_t);
    serializar_lista_de_instrucciones(proceso_actualizado->lista_instrucciones, desplazamiento, stream);
    
    int send_result = send(socket, stream, stream_size , false);
    
    //Liberar memoria
    free(stream);
    //destruir_proceso(proceso_actualizado);


    format_debug_log("conexion_clien.c@enviar_mensaje_proceso_desalojado_interrupt", "Send Result %d", send_result); 
    if (send_result != -1) {
    bool response = recv_ack(socket);

    if (response) {
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Recepcion mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_INTERRUPT");
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Termina mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "-----------------------------------------------------------");
      return response;
    }
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Recepcion mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_INTERRUPT");
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Termina mensaje con error OPCODE_PROCESO_DESALOJADO_INTERRUPT");
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "-----------------------------------------------------------");
    return response;
  }

  error_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Error al enviar mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "Termina mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_interrupt", "-----------------------------------------------------------");
  return false;


}

//OPCODE_PROCESO_DESALOJADO_EXIT

bool enviar_mensaje_proceso_desalojado_exit(pcb_t* proceso_actualizado, int socket){
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "-----------------------------------------------------------");
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Comienza envio de mensaje - OPCODE_PROCESO_DESALOJADO_EXIT");
    void *stream;
    uint32_t stream_size = serializar_proceso(proceso_actualizado,OPCODE_PROCESO_DESALOJADO_EXIT, &stream);
    int send_result = send(socket, stream, stream_size , false);
    
    //Liberar memoria
    free(stream);
    //destruir_proceso(proceso_actualizado);

    format_debug_log("conexion_clien.c@enviar_mensaje_proceso_desalojado_exit", "Send Result %d", send_result); 
    if (send_result != -1) {
    bool response = recv_ack(socket);



    if (response) {
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Recepcion mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_EXIT");
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Termina mensaje OPCODE_PROCESO_DESALOJADO_EXIT");
      info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "-----------------------------------------------------------");
      return response;
    }
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Recepcion mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_EXIT");
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Termina mensaje con error OPCODE_PROCESO_DESALOJADO_EXIT");
    info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "-----------------------------------------------------------");
    return response;
  }

  error_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Error al enviar mensaje OPCODE_PROCESO_DESALOJADO_EXIT");
  debug_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "Termina mensaje con error OPCODE_PROCESO_DESALOJADO_EXIT");
  info_log("conexion_client.c@enviar_mensaje_proceso_desalojado_exit", "-----------------------------------------------------------");
  return false;


}



