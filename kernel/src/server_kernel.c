#include "../include/server_kernel.h"

pcb_t *deserealizar_nuevo_proceso(int socket);

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

  pthread_create(&hilo_server, NULL, (void *)escuchar_conexiones_nuevas, (int *)socket_servidor);

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

pcb_t *deserealizar_nuevo_proceso(int socket) {

  pcb_t *pcb = malloc(sizeof(pcb_t));
  pcb->pid = obtener_siguiente_pid();
  pcb->lista_instrucciones = recibir_lista_de_instrucciones(socket);
  pcb->program_counter = 0;
  pcb->estado = ESTADO_PROCESO_NEW;
  pcb->tabla_paginas = 0; // TODO: validar que esto no sea una tabla valida, quiza un -1? aunque es un uint32_t
  pcb->estimacion_rafaga = kernel_config->estimacion_inicial;
  
  return pcb;
}

bool recibir_mensaje_proceso_nuevo(int cliente_socket) {

  info_log("server_kernel.c@recibir_mensaje_proceso_nuevo","Recibiendo nuevo proceso");
  uint32_t codigo_operacion;

  ssize_t recibido = recv(cliente_socket, &codigo_operacion, sizeof(uint32_t), 0);

  format_debug_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "BYTES RECIBIDOS: %d", recibido);

  if (recibido != sizeof(uint32_t) || codigo_operacion != (uint32_t)OPCODE_NUEVO_PROCESO) {
    error_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "El codOp no corresponde a crear proceso (NUEVO_PROCESO)!");
    return false;
  }

  format_debug_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "Recepcion Op Code Nro %d KERNEL_NUEVO_PROCESO\n", codigo_operacion);

  pcb_t *nuevo_proceso = deserealizar_nuevo_proceso(cliente_socket);

  if (!ejecutar_proceso_nuevo(nuevo_proceso)) {
    //send_ack(cliente_socket, OPCODE_ACK_ERROR);
    return false;
  }

  //send_ack(cliente_socket, OPCODE_ACK_OK);

  //TODO : limpiar *nuevo_proceso y sus propiedades
  //list_destroy_and_destroy_elements(nuevo_proceso)

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

  case OPCODE_PRUEBA: {
    char *mensaje_log =
        string_from_format("Recepcion Op Code Nro %d\n", codigo_operacion);
    info_log("server_kernel.c@procesar_conexion", mensaje_log);
    free(mensaje_log);
    send_ack(cliente_socket, OPCODE_ACK_OK);
    return true;
    break;
  }
  
  // Errores con las conexiones
  case OPCODE_CLIENTE_DESCONECTADO:
    error_log("server_kernel.c@procesar_conexion", "Cliente desconectado de Servidor");
    return false;
  default:

    return false;
    break;
  }

}