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

int iniciar_conexion_cpu_interrupt()
{
  debug_log("conexion_client.c@iniciar_conexion_cpu_interrupt", "Se inicia conexion con CPU - Interrupt");

  socket_cliente_cpu_interrupt = crear_conexion(kernel_config->ip_cpu, kernel_config->puerto_cpu_interrupt, "Interrupt ", "conexion_client.c@iniciar_conexion_cpu_interrupt");

  if (socket_cliente_cpu_interrupt == -1)
  {
    return 0;
  }
  return 1;
}

uint32_t enviar_mensaje_valor_tabla_1er_nivel(uint32_t pid, uint32_t tamanio)
{

  debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "-----------------------------------------------------------");
  debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Serializando y enviando mensaje VALUE_TAB_PAG");
  //CODOP + PID + TAMANIO
  int stream_size = sizeof(op_code_t) + sizeof(uint32_t) + sizeof(uint32_t);
  void *stream = malloc(stream_size);
  op_code_t op_code = OPCODE_VALUE_TAB_PAG;

  memcpy(stream, &op_code, sizeof(op_code_t));
  memcpy(stream + sizeof(op_code_t), &pid, sizeof(uint32_t));
  memcpy(stream + sizeof(uint32_t) + sizeof(uint32_t), &tamanio, sizeof(uint32_t));

  int send_result = send(socket_cliente_kernel, stream, stream_size, false);
  free(stream);

  format_debug_log("conexion_clien.c@enviar_mensaje_exit", "Send Result %d", send_result);
  if (send_result != -1)
  {

    debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Comienza recepcion de mensaje - VALUE_TAB_PAG");
    uint32_t codigo_operacion;
    uint32_t valor_tabla_1er_nivel;

    if (recv(socket_cliente_kernel, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t))
    {
      format_debug_log(".c@enviar_mensaje_valor_tabla_1er_nivel", "Codigo de operacion: %d", codigo_operacion);
      debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "El codOp no corresponde al protocolo de Comunicacion!");
      debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "-----------------------------------------------------------");
      return -1;
    }
    recv(socket_cliente_kernel, &valor_tabla_1er_nivel, sizeof(uint32_t), false);
    format_debug_log(".c@enviar_mensaje_valor_tabla_1er_nivel", "Pid: %d - Valor de tabla de 1er Nivel :%d", pid, valor_tabla_1er_nivel);
    debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Termina mensaje - VALUE_TAB_PAG");
    debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "-----------------------------------------------------------");
    return valor_tabla_1er_nivel;
  }

  error_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "[ERROR] Envio mensaje VALUE_TAB_PAG");
  debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "Termina mensaje VALUE_TAB_PAG");
  debug_log("conexion_client.c@enviar_mensaje_valor_tabla_1er_nivel", "-----------------------------------------------------------");
  return -1;
}

bool enviar_mensaje_exit(pcb_t *proceso)
{
  debug_log("conexion_client.c@enviar_mensaje_exit", "-----------------------------------------------------------");
  debug_log("conexion_client.c@enviar_mensaje_exit", "Comienza envio de mensaje - OPCODE_EXIT");
  //CODOP + PID
  int stream_size = sizeof(op_code_t) + sizeof(uint32_t);
  void *stream = malloc(stream_size);
  op_code_t op_code = OPCODE_EXIT;

  memcpy(stream, &op_code, sizeof(op_code_t));
  memcpy(stream + sizeof(op_code_t), &(proceso->pid), sizeof(uint32_t));

  int send_result = send(socket_cliente_kernel, stream, stream_size, false);

  format_debug_log("conexion_clien.c@enviar_mensaje_exit", "Send Result %d", send_result);

  free(stream);

  if (send_result != -1)
  {
    bool response = recv_ack(socket_cliente_kernel);

    if (response)
    {
      debug_log("conexion_client.c@enviar_mensaje_exit", "Recepcion mensaje ACK OK - OPCODE_EXIT");
      debug_log("conexion_client.c@enviar_mensaje_exit", "Termina mensaje OPCODE_EXIT");
      debug_log("conexion_client.c@enviar_mensaje_exit", "-----------------------------------------------------------");
      return response;
    }
    debug_log("conexion_client.c@enviar_mensaje_exit", "Recepcion mensaje ACK ERROR -OPCODE_EXIT");
    debug_log("conexion_client.c@enviar_mensaje_exit", "Termina mensaje OPCODE_EXIT");
    debug_log("conexion_client.c@enviar_mensaje_exit", "-----------------------------------------------------------");
    return response;
  }

  error_log("conexion_client.c@enviar_mensaje_exit", "[Error] al enviar mensaje OPCODE_EXIT");
  debug_log("conexion_client.c@enviar_mensaje_exit", "Termina mensaje OPCODE_EXIT");
  debug_log("conexion_client.c@enviar_mensaje_exit", "-----------------------------------------------------------");
  return false;
}

bool enviar_mensaje_ejecutar(pcb_t *proceso)
{

  debug_log("conexion_client.c@enviar_mensaje_ejecutar", "-----------------------------------------------------------");
  debug_log("conexion_client.c@enviar_mensaje_ejecutar", "Comienza envio de mensaje - EJECUTAR");
  void *stream;
  uint32_t stream_size = serializar_proceso(proceso, OPCODE_EJECUTAR, &stream);

  int send_result = send(socket_cliente_cpu_dispatch, stream, stream_size, false);
  format_debug_log("conexion_clien.c@enviar_mensaje_ejecutar", "Socket %d", socket);
  free(stream);

  format_debug_log("conexion_clien.c@enviar_mensaje_ejecutar", "Send Result %d", send_result);
  if (send_result != -1)
  {
    bool response = recv_ack(socket_cliente_cpu_dispatch);

    if (response)
    {
      debug_log("conexion_client.c@enviar_mensaje_ejecutar", "Recepcion mensaje ACK OK - EJECUTAR");
      debug_log("conexion_client.c@enviar_mensaje_ejecutar", "Termina mensaje EJECUTAR");
      debug_log("conexion_client.c@enviar_mensaje_ejecutar", "-----------------------------------------------------------");
      return response;
    }
    debug_log("conexion_client.c@enviar_mensaje_ejecutar", "Recepcion mensaje ACK ERROR -EJECUTAR");
    debug_log("conexion_client.c@enviar_mensaje_ejecutar", "Termina mensaje EJECUTAR");
    debug_log("conexion_client.c@enviar_mensaje_ejecutar", "-----------------------------------------------------------");
    return response;
  }

  error_log("conexion_client.c@enviar_mensaje_ejecutar", "Error al enviar mensaje EJECUTAR");
  debug_log("conexion_client.c@enviar_mensaje_ejecutar", "Termina mensaje EJECUTAR");
  debug_log("conexion_client.c@enviar_mensaje_ejecutar", "-----------------------------------------------------------");
  return false;
}

/*Mensaje de desalojo de un proceso por la conexion interrupt - Se envia el pid solo para corroborar que sea el pid que esta en cpu */
bool enviar_mensaje_desalojar_proceso(pcb_t *proceso)
{
  debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "-----------------------------------------------------------");
  debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Comienza envio de mensaje - OPCODE_DESALOJAR_PROCESO");
  int stream_size = sizeof(op_code_t) + sizeof(uint32_t);
  void *stream = malloc(stream_size);
  op_code_t op_code = OPCODE_DESALOJAR_PROCESO;

  memcpy(stream, &op_code, sizeof(op_code_t));
  memcpy(stream + sizeof(op_code_t), &(proceso->pid), sizeof(uint32_t));

  int send_result = send(socket_cliente_cpu_interrupt, stream, stream_size, false);

  format_debug_log("conexion_clien.c@enviar_mensaje_desalojar_proceso", "Send Result %d", send_result);
  if (send_result != -1)
  {
    bool response = recv_ack(socket_cliente_cpu_interrupt);

    if (response)
    {
      debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Recepcion mensaje ACK OK - OPCODE_DESALOJAR_PROCESO");
      debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Termina mensaje OPCODE_DESALOJAR_PROCESO");
      debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "-----------------------------------------------------------");
      return response;
    }
    debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Recepcion mensaje ACK ERROR -OPCODE_DESALOJAR_PROCESO");
    debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Termina mensaje OPCODE_DESALOJAR_PROCESO");
    debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "-----------------------------------------------------------");
    return response;
  }

  error_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Error al enviar mensaje OPCODE_DESALOJAR_PROCESO");
  debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "Termina mensaje OPCODE_DESALOJAR_PROCESO");
  debug_log("conexion_client.c@enviar_mensaje_desalojar_proceso", "-----------------------------------------------------------");
  return false;
}

//OPCODE_SUSPENDER_PROCESO
bool enviar_mensaje_suspender_proceso(pcb_t *proceso)
{
  debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "-----------------------------------------------------------");
  debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Comienza envio de mensaje - OPCODE_SUSPENDER_PROCESO");
  int stream_size = sizeof(op_code_t) + sizeof(uint32_t);
  void *stream = malloc(stream_size);
  op_code_t op_code = OPCODE_SUSPENDER_PROCESO;

  memcpy(stream, &op_code, sizeof(op_code_t));
  memcpy(stream + sizeof(op_code_t), &(proceso->pid), sizeof(uint32_t));

  int send_result = send(socket_cliente_kernel, stream, stream_size, false);

  format_debug_log("conexion_clien.c@enviar_mensaje_suspender_proceso", "Send Result %d", send_result);
  if (send_result != -1)
  {
    bool response = recv_ack(socket_cliente_kernel);

    if (response)
    {
      debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Recepcion mensaje ACK OK - OPCODE_SUSPENDER_PROCESO");
      debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Termina mensaje OPCODE_SUSPENDER_PROCESO");
      debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "-----------------------------------------------------------");
      return response;
    }
    debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Recepcion mensaje ACK ERROR -OPCODE_SUSPENDER_PROCESO");
    debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Termina mensaje OPCODE_SUSPENDER_PROCESO");
    debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "-----------------------------------------------------------");
    return response;
  }

  error_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Error al enviar mensaje OPCODE_SUSPENDER_PROCESO");
  debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "Termina mensaje OPCODE_SUSPENDER_PROCESO");
  debug_log("conexion_client.c@enviar_mensaje_suspender_proceso", "-----------------------------------------------------------");
  return false;
}