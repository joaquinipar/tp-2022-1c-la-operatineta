#include "../include/server_mem.h"

bool enviar_mensaje_inicial_configuracion(int cliente_socket){
  debug_log("server_mem.c@enviar_mensaje_inicial_configuracion", "Solicitud de configuracion inicial CPU - Mensaje PING_PONG_MEMORIA");

    int stream_size = sizeof(uint32_t) + sizeof(uint32_t);
    void *stream = malloc(stream_size);
    //op_code_t op_code = PING_PONG_MEMORIA;
    uint32_t entradas_por_tabla = mem_swap_config->entradas_por_tabla; 
    uint32_t tamanio_pagina = mem_swap_config->tam_pagina; 
   
   // memcpy(stream, &op_code, sizeof(op_code_t));
    memcpy(stream, &entradas_por_tabla, sizeof(uint32_t));
    memcpy(stream + sizeof(uint32_t), &tamanio_pagina, sizeof(uint32_t));
  
    int send_result = send(cliente_socket, stream, stream_size, false);
    free(stream); 

      if (send_result != -1) {
      debug_log("server_mem.c@enviar_mensaje_inicial_configuracion", "Comienza envio de mensaje a CPU - PING_PONG_MEMORIA"); 
      debug_log("server_mem.c@enviar_mensaje_inicial_configuracion", "Termina PING_PONG_MEMORIA");
      return true; 
    }   
    error_log("server_mem.c@enviar_mensaje_inicial_configuracion", "[ERROR] Envio mensaje PING_PONG_MEMORIA a CPU ");
    debug_log("server_mem.c@enviar_mensaje_inicial_configuracion", "Termina mensaje PING_PONG_MEMORIA");
  return false;
}


void iniciar_server_mem_swap(char* ip, char* puerto) {

  debug_log("server_mem.c@iniciar_server_mem_swap", "Inicializando el Servidor Memoria");

  pthread_mutex_init(&sem_procesar_conexion, NULL);

  socket_server_mem= iniciar_servidor("server_mem.c@iniciar_server_mem_swap","", ip, puerto);

  if (socket_server_mem != -1) {
   // info_log("server_mem.c@iniciar_server_mem_swap", "Server Memoria inicializado");
    char *msg_log = string_from_format("Servidor Memoria escuchando conexiones con exito en ip: %s, puerto: %s", ip,puerto);
    info_log("server_mem.c@iniciar_server_mem_swap", msg_log);
    free(msg_log);

  } else {
    char *msg_log1 = string_from_format("No se pudo inicializar el servidor en ip: %s, puerto: %s, revisar que la config sea correcta y esten disponibles.",
                           ip, puerto);
    error_log("server_mem.c@iniciar_server_mem_swap", msg_log1);
    free(msg_log1);
  }

  escuchar_conexiones_nuevas(socket_server_mem);

}


int escuchar_conexiones_nuevas(int server_socket) {

  bool escuchar = true;

  void sighandler(int signal) {
    info_log("server_mem.c@escuchar_conexiones_nuevas","SIGINT recibida, ejecutando handler para apagar la conexion");
    escuchar = false;
    shutdown(server_socket, SHUT_RD);
    close(server_socket);
  }

  signal(SIGINT, sighandler);
  
  while (escuchar == true) {
   // info_log("server_mem.c@escuchar_conexiones_nuevas","Escuchando conexiones nuevas en el Servidor de Memoria");    

    int cliente_socket = esperar_cliente(server_socket, "Memoria", "server.c@escuchar_cliente");

    if (cliente_socket != -1) {
      info_log("server_mem.c@escuchar_conexiones_nuevas","Cliente nuevo conectado");
    hilo_cpu = -1;
    hilo_kernel = -1;

        void loopear_procesar_conexion(int cliente_socket){
            while(procesar_conexion(cliente_socket));
        }


     if(hilo_cpu == -1){
      pthread_create(&hilo_cpu, NULL, (void*) loopear_procesar_conexion, (void*) cliente_socket);
     } else if(hilo_kernel== -1){
       pthread_create(&hilo_kernel, NULL, (void*) loopear_procesar_conexion, (void*) cliente_socket);
     } else {
       error_log("server_mem.c@escuchar_conexiones_nuevas", "ERROR - Ya hay dos clientes conectados"); 
     }

      continue;
    }
    error_log("server_mem.c@escuchar_conexiones_nuevas", "Error en nueva conexion o socket servidor cerrado");
    return -1;
  }

  return 0;
}

bool procesar_conexion(int cliente_socket) {
  

  debug_log("server_mem.c@procesar_conexion", "Procesando nuevo mensaje");
  op_code_t codigo_operacion;

  if (recv(cliente_socket, &codigo_operacion, sizeof(op_code_t), 0) !=
      sizeof(op_code_t)) {
    info_log("server_mem.c@procesar_conexion", "Se recibio un codigo_operacion invalido!");
    return false;
  }

  char *mensaje_recibido_log = string_from_format("OPCODE recibido: %d", codigo_operacion);
  trace_log("server_mem.c@procesar_conexion", mensaje_recibido_log);
  free(mensaje_recibido_log);

  switch (codigo_operacion) {

  case OPCODE_PRUEBA: {
    char *mensaje_log =
        string_from_format("Recepcion Op Code Nro %d\n", codigo_operacion);
    info_log("server_mem.c@procesar_conexion", mensaje_log);
    free(mensaje_log);
    send_ack(cliente_socket, OPCODE_ACK_OK);
    return true;
    break;
  }
  /*Crea las estructuras adm en memoria y devuelve el nro de tabla del 1er nivel"*/
  case OPCODE_VALUE_TAB_PAG:{

  // | codop | pid | tamanio
    uint32_t pid;
    recv(cliente_socket, &pid, sizeof(uint32_t), false);
    uint32_t tamanio;
    recv(cliente_socket, &tamanio, sizeof(uint32_t), false);

    pthread_mutex_lock(&sem_procesar_conexion);
    info_log("server_mem.c@procesar_conexion", "---------------------------------------------------------------------------");

    uint32_t valor_tabla_1er_nivel = admitir_proceso(pid, tamanio);
    // Voy a enviar | CODOP | VALOR_TABLA_1ER_NIVEL |
    format_info_log("server_mem.c@procesar_conexion", "PID: %i Envio valor tabla 1er nivel: %i", pid, valor_tabla_1er_nivel);
    send_codigo_op_con_numero(cliente_socket, OPCODE_VALUE_TAB_PAG, valor_tabla_1er_nivel);
    info_log("server_mem.c@procesar_conexion", "---------------------------------------------------------------------------");

    pthread_mutex_unlock(&sem_procesar_conexion);
    return true;
    break; 
  }
  case OPCODE_ACCESO_1ER_NIVEL:{
      /* Recibo | PID | Posicion de la tabla de 1er nivel |Entrada de 1er nivel | */
      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t posicion_tabla_1er_nivel;
      recv(cliente_socket, &posicion_tabla_1er_nivel, sizeof(uint32_t), false);
      uint32_t numero_entrada_1er_nivel;
      recv(cliente_socket, &numero_entrada_1er_nivel, sizeof(uint32_t), false);

      pthread_mutex_lock(&sem_procesar_conexion);

      info_log("server_mem.c@procesar_conexion", "------------------------------------------");
      format_info_log("server_mem.c@procesar_conexion", "PID: %i Posicion tabla 1er nivel: %i Numero entrada: %i", pid, posicion_tabla_1er_nivel, numero_entrada_1er_nivel);
      uint32_t numero_tabla_2do_nivel =  buscar_nro_tabla_2do_nivel( pid, posicion_tabla_1er_nivel, numero_entrada_1er_nivel);
      format_info_log("server_mem.c@procesar_conexion", "PID: %i Valor tabla 2do nivel: %i", pid, numero_tabla_2do_nivel);
      /* Envio | CODOP | PID | numero_tabla_2do_nivel */

      usleep(mem_swap_config->retardo_memoria *1000);

      info_log("server_mem.c@procesando_conexion", "Ejecutando retardo del ACCESO DE 1ER NIVEL");
      int res = send_codigo_op_con_numeros(cliente_socket, OPCODE_ACCESO_1ER_NIVEL, pid, numero_tabla_2do_nivel);

      if(res != 1){
          error_log("server_mem.c@procesar_conexion", "Ocurrió un error al enviar la respuesta de ACCESO_1ER_NIVEL");
      }

      pthread_mutex_unlock(&sem_procesar_conexion);
      return true;
      break;
  }
  case OPCODE_ACCESO_2DO_NIVEL:{

      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t nro_tabla_2do_nivel;
      recv(cliente_socket, &nro_tabla_2do_nivel, sizeof(uint32_t), false);
      uint32_t nro_entrada_2do_nivel;
      recv(cliente_socket, &nro_entrada_2do_nivel, sizeof(uint32_t), false);

      pthread_mutex_lock(&sem_procesar_conexion);

      format_warning_log("server_mem.c@procesar_conexion", "(OPCODE_ACCESO_2DO_NIVEL) Recibi PID: %i nro_tabla_2do_nivel: %i nro_entrada_2do_nivel: %i", pid, nro_tabla_2do_nivel, nro_entrada_2do_nivel);

      uint32_t marco = obtener_marco_de_tabla_2do_nivel(pid, nro_tabla_2do_nivel, nro_entrada_2do_nivel);
      format_warning_log("server_mem.c@procesar_conexion", "(OPCODE_ACCESO_2DO_NIVEL) PID: %i Envio respuesta MARCO: ", pid, marco);

      if(obtener_puntero_clock(pid) == -1){ // Si es la primera vez que escribe, le seteo el puntero clock al marco.
          setear_marco_a_puntero_clock(pid, marco);
      }

      info_log("server_mem.c@procesando_conexion", "Ejecutando retardo del ACCESO DE 2DO NIVEL");
      usleep(mem_swap_config->retardo_memoria *1000);

      int res = send_codigo_op_con_numeros(cliente_socket, OPCODE_ACCESO_2DO_NIVEL, pid, marco);
      if(res != 1){
          error_log("server_mem.c@procesar_conexion", "Ocurrió un error al enviar la respuesta de ACCESO_2DO_NIVEL");
      }
      pthread_mutex_unlock(&sem_procesar_conexion);

      return true;
      break;
  }
  case OPCODE_READ: {
      /* Recibo READ [ CODOP | PID | Dir_fisica ] */
      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t direccion_fisica;
      recv(cliente_socket, &direccion_fisica, sizeof(uint32_t), false);
      info_log("server_mem.c@procesando_conexion","------------------------------------------");
      format_info_log("server_mem.c@procesando_conexion", "PID: %i READ DF: %i", pid, direccion_fisica);

      uint32_t* lectura = leer(direccion_fisica);

      imprimir_estado_array_MP();

      info_log("server_mem.c@procesando_conexion", "Ejecutando retardo del READ");
      usleep(mem_swap_config->retardo_memoria *1000);

      int res = send_codigo_op_con_numeros(cliente_socket, OPCODE_READ, pid, *lectura);

      info_log("server_mem.c@procesar_conexion", "Envie el contenido");

      if(res != 1){
          error_log("server_mem.c@procesar_conexion", "Ocurrió un error al enviar la respuesta de READ");
      }
      info_log("server_mem.c@procesando_conexion","------------------------------------------");
      free(lectura);
      return true;
      break;
  }
  case OPCODE_WRITE: {
      uint32_t pid;
      recv(cliente_socket, &pid, sizeof(uint32_t), false);
      uint32_t direccion_fisica;
      recv(cliente_socket, &direccion_fisica, sizeof(uint32_t), false);
      uint32_t contenido; // igual sabemos que siempre es un uint32_t
      recv(cliente_socket, &contenido, sizeof(uint32_t), false);

      pthread_mutex_lock(&sem_procesar_conexion);
      info_log("server_mem.c@procesando_conexion","------------------------------------------");
      format_info_log("server_mem.c@procesando_conexion", "PID: %i WRITE DF: %i Contenido: %i", pid, direccion_fisica, contenido);
      escribir(direccion_fisica, contenido);

      // todo Actualizar bit de modificado. calcular marco con DF. Direccion fisica / tamaño de pagina
      int marco = direccion_fisica / mem_swap_config->tam_pagina;
      format_info_log("server_mem.c@procesando_conexion", "PID: %i Marco: %i Seteo Bit Modificado en 1", pid, marco);
      array_marcos[marco].pagina->bit_modificado = 1;

      imprimir_estado_array_MP();

      info_log("server_mem.c@procesando_conexion", "Ejecutando retardo del WRITE");
      usleep(mem_swap_config->retardo_memoria *1000);

      send_ack(cliente_socket, OPCODE_ACK_OK);
      format_info_log("server_mem.c@procesando_conexion","PID: %i Envie ACK OK", pid);
      info_log("server_mem.c@procesando_conexion","------------------------------------------");
      pthread_mutex_unlock(&sem_procesar_conexion);

      return true;
      break;
  }
  case OPCODE_PING_PONG_MEMORIA: {
    format_info_log("server_mem.c@procesar_conexion", "Se envia mensaje a CPU -- Entradas por tabla: %d - Tamaño de pagina: %d", mem_swap_config->entradas_por_tabla,mem_swap_config->tam_pagina); 
    send_codigo_op_con_numeros(cliente_socket,OPCODE_PING_PONG_MEMORIA, mem_swap_config->entradas_por_tabla, mem_swap_config->tam_pagina);
    return true;
    break;
  }
  case __ABORT__: {
      /* Solo para tests unitarios */
      //abort();
      warning_log("server_mem.c@procesar_conexion", "LLEGO EL ABORT!!!");
      cerrar_mem_swap();
      //shutdown(socket_server_mem, SHUT_RD);
      raise(SIGINT);
      return false ;
      break;
  }
  case OPCODE_SUSPENDER_PROCESO: {
    uint32_t pid;
    recv(cliente_socket, &pid, sizeof(uint32_t), false);

    pthread_mutex_lock(&sem_procesar_conexion);
    info_log("server_mem.c@procesando_conexion","------------------------------------------");

    bool response = suspender_proceso(pid);
    if (response) {
      format_info_log("server_mem.c@procesando_conexion", "PID: %i Suspension exitosa", pid);
      format_info_log("server_mem.c@procesando_conexion","PID: %i Envie ACK OK", pid);
      send_ack(cliente_socket, OPCODE_ACK_OK);
      pthread_mutex_unlock(&sem_procesar_conexion);
      return true;
      break; 
    }
    format_info_log("server_mem.c@procesando_conexion", "PID: %i Suspension fallida", pid);
    format_info_log("server_mem.c@procesando_conexion","PID: %i Envie ACK ERROR", pid);
    send_ack(cliente_socket, OPCODE_ACK_ERROR);
    info_log("server_mem.c@procesando_conexion","------------------------------------------");
    pthread_mutex_unlock(&sem_procesar_conexion);
    return true;
    break; 
  }

  case OPCODE_EXIT:{
    uint32_t pid;
    recv(cliente_socket, &pid, sizeof(uint32_t), false);

    pthread_mutex_lock(&sem_procesar_conexion);

    info_log("server_mem.c@procesando_conexion","------------------------------------------");
    format_info_log("server_mem.c@procesando_conexion", "PID: %i Comienza la salida del proceso", pid);

    bool response = cerrar_proceso(pid);
    if (response) {
      format_info_log("server_mem.c@procesando_conexion", "PID: %i Salida exitosa del proceso", pid);
      format_info_log("server_mem.c@procesando_conexion", "PID: %i Envie ACK OK", pid);
      send_ack(cliente_socket, OPCODE_ACK_OK);
      pthread_mutex_unlock(&sem_procesar_conexion);
      return true;
      break; 
    }
    format_info_log("server_mem.c@procesando_conexion", "PID: %i Salida fallida del proceso", pid);
    format_info_log("server_mem.c@procesando_conexion", "PID: %i Envie ACK ERROR", pid);
    send_ack(cliente_socket, OPCODE_ACK_ERROR);

    info_log("server_mem.c@procesando_conexion","------------------------------------------");
    pthread_mutex_unlock(&sem_procesar_conexion);
    return true;
    break; 
  }
  // Errores con las conexiones
  case OPCODE_CLIENTE_DESCONECTADO:
    info_log("server_mem.c@procesar_conexion", "------------------------------------------");
    error_log("server_mem.c@procesar_conexion","Cliente desconectado de Servidor Memoria");
    info_log("server_mem.c@procesar_conexion", "------------------------------------------");
    return false;
  default:
    error_log("server_mem.cc@procesar_conexion","Algo anduvo mal en el Servidor de Memoria");
    return false;
    break;
  }
}


void cerrar_server_memoria(){
  liberar_conexion(&socket_server_mem,"server_mem.c@cerrar_server_memoria"); 
}

