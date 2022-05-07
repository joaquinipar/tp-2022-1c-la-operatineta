#include "../include/server_interrupt.h"

pthread_t iniciar_server_interrupt() {
pthread_t hilo_interrupt;
  char *ip = cpu_config->ip_escucha;
  char *puerto = cpu_config->puerto_escucha_interrupt;

  debug_log("server_interrupt.c@iniciar_server_interrupt", "Inicializando el server Interrupt");

  int socket_servidor_interrupt = iniciar_servidor("server_interrupt.c@iniciar_server_interrupt", "", ip, puerto);

  char *msg_log;

  if (socket_servidor_interrupt != -1) {
    msg_log = string_from_format(
        "Servidor Interrupt escuchando conexiones con exito en ip: %s, puerto: %s", ip,
        puerto);
    info_log("server_interrupt.c@iniciar_server_interrupt", msg_log);
  } else {
    msg_log = string_from_format("No se pudo inicializar el servidor en ip: "
                                 "%s, puerto: %s, revisar que la config sea "
                                 "correcta y esten disponibles.",
                                 ip, puerto);
    error_log("server_interrupt.c@iniciar_server_interrupt", msg_log);
  }

  free(msg_log);

  pthread_create(&hilo_interrupt, NULL, (void *)escuchar_conexiones_nuevas_interrupt,
                 (void *)socket_servidor_interrupt);

  //debug_log("server_interrupt.c@iniciar_server_interrupt", "Server inicializado");

  return hilo_interrupt;

}



int escuchar_conexiones_nuevas_interrupt(int server_socket) {

  bool escuchar = true;

  void sighandler(int signal) {
    info_log("server_interrupt.c@escuchar_conexiones_nuevas_interrupt","SIGINT recibida, ejecutando handler para apagar la conexion Interrupt");
    escuchar = false;
    shutdown(server_socket, SHUT_RD);
    close(server_socket);
  }

  signal(SIGINT, sighandler);


  while (escuchar == true) {
    info_log("server_interrupt.c@escuchar_conexiones_nuevas_interrupt","Escuchando conexiones nuevas en server Interrupt");    

    int cliente_socket = esperar_cliente(server_socket, "Interrupt", "server.c@esperar_cliente");
    

    if (cliente_socket != -1) {
      info_log("server_interrupt.c@escuchar_conexiones_nuevas_interrupt","Cliente nuevo conectado");
      while (procesar_conexion_interrupt(cliente_socket))
        ;
      continue;
    }

    error_log("server_interrupt.c@escuchar_conexiones_nuevas_interrupt", "Error en nueva conexion o socket servidor cerrado");

    return -1;
  }

  return 0;
}

bool procesar_conexion_interrupt(int cliente_socket) {
  debug_log("server_interrupt.c@procesar_conexion_interrupt", "Procesando nuevo mensaje");
  op_code_t codigo_operacion;

  if (recv(cliente_socket, &codigo_operacion, sizeof(op_code_t), 0) !=
      sizeof(op_code_t)) {
    info_log("server_interrupt.c@procesar_conexion_interrupt","Server Interrupt - Se recibio un codigo_operacion invalido!");
    return false;
  }

  char *mensaje_recibido_log =
      string_from_format("Server Interrupt - CodOp recibido: %d", codigo_operacion);
  trace_log("server_interrupt.c@procesar_conexion_interrupt", mensaje_recibido_log);
  free(mensaje_recibido_log);


  switch (codigo_operacion) {

  case OPCODE_PRUEBA: {
    char *mensaje_log =
        string_from_format("Server Interrupt - Recepcion Op Code Nro %d\n", codigo_operacion);
    info_log("server_interrupt.c@procesar_conexion_interrupt", mensaje_log);
    free(mensaje_log);
    send_ack(cliente_socket, OPCODE_ACK_OK);
    return true;
    break;
  }

  // Errores con las conexiones
  case OPCODE_CLIENTE_DESCONECTADO:
    error_log("server_interrupt.c@procesar_conexion_interrupt","Cliente desconectado de Server Interrupt - CPU");
    return false;
  default:
    error_log("server_interrupt.c@procesar_conexion_interrupt","Algo anduvo mal en el Server Interrupt de CPU");
    return false;
    break;
  }
}