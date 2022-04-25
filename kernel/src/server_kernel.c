#include "../include/server_kernel.h"

pthread_t iniciar_server_kernel() {
  pthread_t hilo_server;
  char *ip = kernel_config->ip_escucha;
  char *puerto = kernel_config->puerto_escucha;

  debug_log("server_kernel.c@iniciar_server_kernel", "Inicializando el server Kernel");

  int socket_servidor = iniciar_servidor("server_kernel.c@iniciar_server_kernel", "", ip, puerto);

  char *msg_log;

  if (socket_servidor != -1) {
    msg_log = string_from_format(
        "Servidor Kernel escuchando conexiones con exito en ip: %s, puerto: %s", ip, puerto);
    info_log("server_kernel.c@iniciar_server_kernel", msg_log);
  } else {
    msg_log = string_from_format("No se pudo inicializar el servidor en ip: %s, puerto: %s, revisar que la config sea correcta y esten disponibles.",ip, puerto);
    error_log("server_kernel.c@iniciar_server_kernel", msg_log);
  }

  free(msg_log);

  pthread_create(&hilo_server, NULL, (void *)escuchar_conexiones_nuevas, (void *)socket_servidor);


  return hilo_server;
}

int escuchar_conexiones_nuevas(int server_socket) {
  bool escuchar = true;

  void sighandler(int signal) {
    info_log("server_kernel.c@escuchar_conexiones_nuevas","SIGINT recibida, ejecutando handler para apagar la conexion Kernel");
    escuchar = false;
    shutdown(server_socket, SHUT_RD);
    close(server_socket);
  }

  signal(SIGINT, sighandler);

  debug_log("server_kernel.c@escuchar_conexiones_nuevas","Escuchando conexiones nuevas");

  while (escuchar == true) {
    int cliente_socket = esperar_cliente(server_socket, "Kernel", "server_kernel.c@escuchar");

    if (cliente_socket != -1) {
      bool resultado_crear_proceso =
          recibir_mensaje_proceso_nuevo(cliente_socket);
        debug_log("server_kernel.c@escuchar_conexiones_nuevas", "Cliente nuevo conectado");
      if (!resultado_crear_proceso) {
        error_log("server_kernel.c@escuchar_conexiones_nuevas",
                  "No se pudo crear el proceso nuevo");
      }

      continue;
    }

    debug_log("server_kernel.c@escuchar_conexiones_nuevas",
              "Error en nueva conexion o socket servidor cerrado, saliendo del "
              "thread");

    return -1;
  }

  return 0;
}

bool recibir_mensaje_proceso_nuevo(int cliente_socket) {
  debug_log("server_kernel.c@recibir_mensaje_proceso_nuevo","Procesando nuevo proceso");
  uint32_t codigo_operacion;

  ssize_t recibido = recv(cliente_socket, &codigo_operacion, sizeof(uint32_t), 0);

  format_info_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "BYTES RECIBIDOS: %d", recibido);

  if (recibido != sizeof(uint32_t) || codigo_operacion != (uint32_t)NUEVO_PROCESO) {
    error_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "El codOp no corresponde a crear proceso (NUEVO_PROCESO)!");
    return false;
  }

  char *mensaje_log = string_from_format("Recepcion Op Code Nro %d KERNEL_NUEVO_PROCESO\n", codigo_operacion);
  info_log("server_kernel.c@recibir_mensaje_proceso_nuevo", mensaje_log);
  free(mensaje_log);

  uint32_t pid;
  recv(cliente_socket, &pid, sizeof(uint32_t), false);

  mensaje_log = string_from_format("Nuevo proceso con PID: %d recibido", pid);
  info_log("server_kernel.c@recibir_mensaje_proceso_nuevo", mensaje_log);
  free(mensaje_log);

  if (!admitir_proceso_nuevo(pid, cliente_socket)) {
    send_ack(cliente_socket, ACK_ERROR);
    return false;
  }

  send_ack(cliente_socket, ACK_OK);
  return true;
}

bool procesar_conexion(int cliente_socket) {
  debug_log("server_kernel.c@procesar_conexion", "Procesando nuevo mensaje");
  op_code_t codigo_operacion;

  if (recv(cliente_socket, &codigo_operacion, sizeof(op_code_t), 0) !=
      sizeof(op_code_t)) {
    info_log("server_kernel.c@procesar_conexion",
             "Se recibio un codigo_operacion invalido!");
    return false;
  }

  char *mensaje_recibido_log =
      string_from_format("CodOp recibido: %d", codigo_operacion);
  debug_log("server_kernel.c@procesar_conexion", mensaje_recibido_log);
  free(mensaje_recibido_log);

  switch (codigo_operacion) {

  case PRUEBA: {
    char *mensaje_log =
        string_from_format("Recepcion Op Code Nro %d\n", codigo_operacion);
    info_log("server_kernel.c@procesar_conexion", mensaje_log);
    free(mensaje_log);
    send_ack(cliente_socket, ACK_OK);
    return true;
    break;
  }

  // Errores con las conexiones
  case CLIENTE_DESCONECTADO:
    error_log("server_kernel.c@procesar_conexion", "Cliente desconectado de Servidor");
    return false;
  default:

    return false;
    break;
  }

}