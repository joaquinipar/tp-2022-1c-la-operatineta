#include "../include/server_mem_swap.h"

void iniciar_server_mem_swap() {

  char *ip = mem_swap_config->ip_escucha;
  char *puerto = mem_swap_config->puerto_escucha;

  info_log("server_mem_swap.c@iniciar_server_mem_swap", "Inicializando el Servidor Memoria");

  socket_server_mem= iniciar_servidor("server_mem_swap.c@iniciar_server_mem_swap","", ip, puerto);

  if (socket_server_mem != -1) {
    debug_log("server_mem_swap.c@iniciar_server_mem_swap", "Server Memoria inicializado");
    char *msg_log = string_from_format("Servidor Memoria escuchando conexiones con exito en ip: %s, puerto: %s", ip,puerto);
    info_log("server_mem_swap.c@iniciar_server_mem_swap", msg_log);
    free(msg_log);

  } else {
    char *msg_log1 = string_from_format("No se pudo inicializar el servidor en ip: %s, puerto: %s, revisar que la config sea correcta y esten disponibles.",
                           ip, puerto);
    error_log("server_mem_swap.c@iniciar_server_mem_swap", msg_log1);
    free(msg_log1);
  }

  escuchar_conexiones_nuevas(socket_server_mem);

}


int escuchar_conexiones_nuevas(int server_socket) {

  bool escuchar = true;

  void sighandler(int signal) {
    info_log("server_mem_swap.c@escuchar_conexiones_nuevas","SIGINT recibida, ejecutando handler para apagar la conexion");
    escuchar = false;
    shutdown(server_socket, SHUT_RD);
    close(server_socket);
  }

  signal(SIGINT, sighandler);


  while (escuchar == true) {
    info_log("server_mem_swap.c@escuchar_conexiones_nuevas","Escuchando conexiones nuevas");    

    int cliente_socket = esperar_cliente(server_socket, "Memoria", "server.c@escuchar_cliente");
    

    if (cliente_socket != -1) {
      info_log("server_mem_swap.c@escuchar_conexiones_nuevas","Cliente nuevo conectado");
      while (procesar_conexion(cliente_socket))
        ;
      continue;
    }

    error_log("server_mem_swap.c@escuchar_conexiones_nuevas", "Error en nueva conexion o socket servidor cerrado");

    return -1;
  }

  return 0;
}

bool procesar_conexion(int cliente_socket) {
  debug_log("server_swamp.c@procesar_conexion", "Procesando nuevo mensaje");
  op_code_t codigo_operacion;

  if (recv(cliente_socket, &codigo_operacion, sizeof(op_code_t), 0) !=
      sizeof(op_code_t)) {
    info_log("server_swamp.c@procesar_conexion",
             "Se recibio un codigo_operacion invalido!");
    return false;
  }

  char *mensaje_recibido_log =
      string_from_format("CodOp recibido: %d", codigo_operacion);
  trace_log("server_swamp.c@procesar_conexion", mensaje_recibido_log);
  free(mensaje_recibido_log);



  switch (codigo_operacion) {

  case PRUEBA: {
    char *mensaje_log =
        string_from_format("Recepcion Op Code Nro %d\n", codigo_operacion);
    info_log("server_swamp.c@procesar_conexion", mensaje_log);
    free(mensaje_log);
    send_ack(cliente_socket, ACK_OK);
    return true;
    break;
  }

  // Errores con las conexiones
  case CLIENTE_DESCONECTADO:
    error_log("server_swamp.c@procesar_conexion_swamp",
              "Cliente desconectado de Servidor Memoria");
    return false;
  default:
    error_log("server_swamp.cc@procesar_conexion_swamp",
              "Algo anduvo mal en el Servidor de Swamp");
    return false;
    break;
  }
}

