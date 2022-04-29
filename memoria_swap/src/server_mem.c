#include "../include/server_mem.h"

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
  /*Crea las estructuras adm en memoria y devuelve el nro de tabla del 1er nivel"*/
  case VALUE_TAB_PAG:{
  // | codop | pid | tamanio
    uint32_t pid;
    recv(cliente_socket, &pid, sizeof(uint32_t), false);
    uint32_t tamanio;
    recv(cliente_socket, &tamanio, sizeof(uint32_t), false);
    uint32_t valor_tabla_1er_nivel = inicio_proceso(pid, tamanio);
    // Voy a enviar | CODOP | VALOR_TABLA_1ER_NIVEL |
    send_codigo_op_con_numero(cliente_socket, VALUE_TAB_PAG, valor_tabla_1er_nivel);
    return true;
    break; 
  }
  case ACCESO_1ER_NIVEL:{
      /* Recibo | PID | Posicion de la tabla de 1er nivel |Entrada de 1er nivel | */
      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t posicion_tabla_1er_nivel;
      recv(cliente_socket, &posicion_tabla_1er_nivel, sizeof(uint32_t), false);
      uint32_t numero_entrada_1er_nivel;
      recv(cliente_socket, &numero_entrada_1er_nivel, sizeof(uint32_t), false);

      uint32_t numero_tabla_2do_nivel =  buscar_nro_tabla_2do_nivel( pid, posicion_tabla_1er_nivel, numero_entrada_1er_nivel);

      /* Envio | CODOP | PID | numero_tabla_2do_nivel */

      int res = send_codigo_op_con_numero(ACCESO_1ER_NIVEL, pid, numero_tabla_2do_nivel);

      if(res != 1){
          error_log("server_mem.c", "Ocurrió un error al enviar la respuesta de ACCESO_1ER_NIVEL");
      }

      return true;
      break;
  }
  case READ: {
      /* Recibo READ [ CODOP | PID | Dir_fisica ] */
      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t direccion_fisica;
      recv(cliente_socket, &direccion_fisica, sizeof(uint32_t), false);

      void* lectura = leer(direccion_fisica);

      int res = send_codigo_op_con_numero(READ, pid, (uint32_t)lectura);

      format_debug_log("server_mem.c@procesar_conexion", "Envie el contenido: %d", (uint32_t) lectura);

      if(res != 1){
          error_log("server_mem.c", "Ocurrió un error al enviar la respuesta de READ");
      }

      free(lectura);

      return true;
      break;
  }
  case WRITE: {
      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t direccion_fisica;
      recv(cliente_socket, &direccion_fisica, sizeof(uint32_t), false);
      void* contenido; // igual sabemos que siempre es un uint32_t
      recv(cliente_socket, &contenido, sizeof(uint32_t), false);

      escribir(direccion_fisica, contenido);

      send_ack(cliente_socket, ACK_OK);

      return true;
      break;
  }
  case PING_PONG_MEMORIA: {

      send_codigo_op_con_numeros(cliente_socket,PING_PONG_MEMORIA, mem_swap_config->entradas_por_tabla, mem_swap_config->tam_pagina);
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
