#include "../include/server_kernel.h"

int *socket_servidor;
bool escuchar = true;
void escuchar_mensajes_cliente(int *socket_cliente);

pthread_t iniciar_server_kernel()
{
  pthread_t hilo_server;
  char *ip = kernel_config->ip_escucha;
  char *puerto = kernel_config->puerto_escucha;

  debug_log("server_kernel.c@iniciar_server_kernel", "Inicializando el server Kernel");

  socket_servidor = malloc(sizeof(int));

  *socket_servidor = iniciar_servidor("server_kernel.c@iniciar_server_kernel", "", ip, puerto);

  char *msg_log;

  if (*socket_servidor != -1)
  {
    msg_log = string_from_format(
        "Servidor Kernel escuchando conexiones con exito en ip: %s, puerto: %s", ip, puerto);
    info_log("server_kernel.c@iniciar_server_kernel", msg_log);
    info_log("-","-----------------------------------------------------------------------------------------------------"); 
  }
  else
  {
    msg_log = string_from_format("No se pudo inicializar el servidor en ip: %s, puerto: %s, revisar que la config sea correcta y esten disponibles.", ip, puerto);
    error_log("server_kernel.c@iniciar_server_kernel", msg_log);
  }

  free(msg_log);

  pthread_create(&hilo_server, NULL, (void *)escuchar_conexiones_nuevas, socket_servidor);
  //pthread_detach(hilo_server);

  return hilo_server;
}

void cerrar_server_kernel(int signal){
  info_log("server_kernel.c@escuchar_conexiones_nuevas", "SIGINT recibida, ejecutando handler para apagar la conexion Kernel");
  escuchar = false;
  shutdown(*socket_servidor, SHUT_RD);
  close(*socket_servidor);
  free(socket_servidor);
}

int escuchar_conexiones_nuevas(int *server_socket)
{
  

  debug_log("server_kernel.c@escuchar_conexiones_nuevas", "Escuchando conexiones nuevas");

  while (escuchar == true)
  {
    pthread_t thread_cliente;
    int *socket_cliente = malloc(sizeof(int));
    *socket_cliente = esperar_cliente(*server_socket, "Kernel", "server_kernel.c@escuchar");

    if (*socket_cliente != -1)
    {
      pthread_create(&thread_cliente, NULL, (void *)escuchar_mensajes_cliente, socket_cliente);
      pthread_detach(thread_cliente);
      free(socket_cliente);
      continue;
    }

    debug_log("server_kernel.c@escuchar_conexiones_nuevas",
              "Error en nueva conexion o socket servidor cerrado, saliendo del thread");
    free(socket_cliente);
    return -1;
  }

  return 0;
}

void escuchar_mensajes_cliente(int *socket_cliente) {
  bool resultado_crear_proceso = recibir_mensaje_proceso_nuevo(socket_cliente);
  debug_log("server_kernel.c@escuchar_conexiones_nuevas", "Cliente nuevo conectado");
  if (!resultado_crear_proceso)
  {
    error_log("server_kernel.c@escuchar_conexiones_nuevas", "No se pudo crear el proceso nuevo");
  }
}

pcb_t *deserealizar_nuevo_proceso(int *socket)
{

  pcb_t *pcb = malloc(sizeof(pcb_t));
  
 // format_debug_log("server_kernel.c@desearilizar_nuevo_proceso", "Tamaño proceso: %d", pcb->tamanio);
  pcb->pid = obtener_siguiente_pid();
  pcb->lista_instrucciones = recibir_lista_de_instrucciones(*socket);
  pcb->program_counter = 0;
  pcb->estado = ESTADO_PROCESO_NEW;
  pcb->tabla_paginas = -1;
  pcb->estimacion_rafaga = kernel_config->estimacion_inicial;
  recv(*socket, &(pcb->tamanio), sizeof(uint32_t), 0);
  format_debug_log("server_kernel.c@desearilizar_nuevo_proceso", "Tamaño proceso: %d", pcb->tamanio);
  pcb->socket = *socket;
  //void printear_instrucciones(instruccion_t * una_instruccion)
  //{
  //  format_debug_log("serializacion.c@printear_lista", "Instruccion(numero): %d", una_instruccion->instruccion);
  //  format_debug_log("serializacion.c@printear_lista", "argumento 1: %d", una_instruccion->argumentos[0]);
  //  format_debug_log("serializacion.c@printear_lista", "argumento 2: %d", una_instruccion->argumentos[1]);
  //}
  //list_iterate(pcb->lista_instrucciones, (void *)printear_instrucciones);

  return pcb;
}

bool recibir_mensaje_proceso_nuevo(int *cliente_socket)
{

  info_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "Recibiendo nuevo proceso");
  op_code_t codigo_operacion;

  ssize_t recibido = recv(*cliente_socket, &codigo_operacion, sizeof(op_code_t), 0);

  format_debug_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "BYTES RECIBIDOS: %d", recibido);
  format_debug_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "codOp: %d", codigo_operacion);

  if (recibido != sizeof(op_code_t) || codigo_operacion != OPCODE_NUEVO_PROCESO)
  {
    error_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "El codOp no corresponde a crear proceso (NUEVO_PROCESO)!");
    return false;
  }

  format_debug_log("server_kernel.c@recibir_mensaje_proceso_nuevo", "Recepcion Op Code Nro %d KERNEL_NUEVO_PROCESO\n", codigo_operacion);

  pcb_t *nuevo_proceso = deserealizar_nuevo_proceso(cliente_socket);

  if (!ejecutar_proceso_nuevo(nuevo_proceso))
  {
    //send_ack(cliente_socket, OPCODE_ACK_ERROR);
    return false;
  }

  //send_ack(cliente_socket, OPCODE_ACK_OK);

  //TODO : limpiar *nuevo_proceso y sus propiedades
  //proceso_destruir(nuevo_proceso);

  return true;
}