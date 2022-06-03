#include "../include/server_dispatch.h"

int cliente_socket;

pthread_t iniciar_server_dispatch()
{
  pthread_t hilo_dispatch;
  char *ip = cpu_config->ip_escucha;
  char *puerto = cpu_config->puerto_escucha_dispatch;

  debug_log("server_dispatch.c@iniciar_server_dispatch", "Inicializando el server Dispatch");

  int socket_server_dispatch = iniciar_servidor("server_dispatch.c@iniciar_server_dispatch", "", ip, puerto);

  char *msg_log;

  if (socket_server_dispatch != -1) {
    msg_log = string_from_format(
        "Servidor Dispatch escuchando conexiones con exito en ip: %s, puerto: %s", ip,
        puerto);
    info_log("server_dispatch.c@iniciar_server_dispatch", msg_log);
  }
  else
  {
    msg_log = string_from_format("No se pudo inicializar el servidor en ip: "
                                 "%s, puerto: %s, revisar que la config sea "
                                 "correcta y esten disponibles.",
                                 ip, puerto);
    error_log("server_dispatch.c@iniciar_server_dispatch", msg_log);
  }

  free(msg_log);

  pthread_create(&hilo_dispatch, NULL, (void *)escuchar_conexiones_nuevas_dispatch,
                 (void *)socket_server_dispatch);

  //debug_log("server_dispatch.c@iniciar_server_dispatch", "Server inicializado");

  return hilo_dispatch;
}

int escuchar_conexiones_nuevas_dispatch(int server_socket)
{

  bool escuchar = true;

  void sighandler(int signal)
  {
    info_log("server_dispatch.c@escuchar_conexiones_nuevas_dispatch", "SIGINT recibida, ejecutando handler para apagar la conexion Dispatch");
    escuchar = false;
    shutdown(server_socket, SHUT_RD);
    shutdown(server_socket, SHUT_RD);
    close(server_socket);
  }

  signal(SIGINT, sighandler);

  while (escuchar == true)
  {
    info_log("server_dispatch.c@escuchar_conexiones_nuevas_dispatch", "Escuchando conexiones nuevas en server Dispatch");

    cliente_socket = esperar_cliente(server_socket, "Dispatch", "server.c@esperar_cliente");

    if (cliente_socket != -1)
    {
      info_log("server_dispatch.c@escuchar_conexiones_nuevas_dispatch", "Cliente nuevo conectado");
      while (procesar_conexion_dispatch(cliente_socket))
        ;
      continue;
    }

    error_log("server_dispatch.c@escuchar_conexiones_nuevas_dispatch", "Error en nueva conexion o socket servidor cerrado");

    return -1;
  }

  return 0;
}

bool procesar_conexion_dispatch(int cliente_socket)
{
  debug_log("server_dispatch.c@procesar_conexion_dispatch", "Procesando nuevo mensaje");
  op_code_t codigo_operacion;

  if (recv(cliente_socket, &codigo_operacion, sizeof(op_code_t), 0) != sizeof(op_code_t))
  {
    info_log("server_dispatch.c@procesar_conexion_dispatch", "Server Dispatch - Se recibio un codigo_operacion invalido!");
    return false;
  }

  format_debug_log("server_dispatch.c@procesar_conexion_dispatch", "Server Dispatch - CodOp recibido: %d", codigo_operacion);

  switch (codigo_operacion) {

  case OPCODE_PRUEBA:
  {
    char *mensaje_log =
        string_from_format("Server Dispatch - Recepcion Op Code Nro %d\n", codigo_operacion);
    info_log("server_dispatch.c@procesar_conexion_dispatch", mensaje_log);
    free(mensaje_log);
    send_ack(cliente_socket, OPCODE_ACK_OK);
    return true;
    break;
  }

  case OPCODE_EJECUTAR:
  {
    format_debug_log("server_dispatch.c@procesar_conexion", "CodOP recibido %d", codigo_operacion);

    pcb_t *proceso_recibido = deserializar_proceso(cliente_socket);
    send_ack(cliente_socket, OPCODE_ACK_OK);

    iniciar_modulo(proceso_recibido);

    return true;
    break;
  }

  // Errores con las conexiones
  case OPCODE_CLIENTE_DESCONECTADO:
    error_log("server_dispatch.c@procesar_conexion_dispatch", "Cliente desconectado de Server Dispatch - CPU");
    return false;

  default:
    error_log("server_dispatch.c@procesar_conexion_dispatch", "El codop no coincide con ninguno de los que este server escucha. Algo anduvo mal en el Server Dispatch de CPU");
    return false;
    break;
  }
}