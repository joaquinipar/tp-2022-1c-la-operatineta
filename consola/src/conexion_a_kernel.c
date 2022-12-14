#include "../include/conexion_a_kernel.h"

bool enviar_proceso_nuevo(int socket, void* stream, uint32_t tamanio_stream) {
  format_debug_log("conexion_a_kernel.c@enviar_proceso_nuevo", "Tamaño del stream %d", tamanio_stream); 

  int send_result = send(socket, stream, tamanio_stream, false);
  format_debug_log("conexion_a_kernel.c@enviar_proceso_nuevo", "Send Result %d", send_result); 

  if (send_result != -1) {
    // Esperar a que el kernel responda por la inicializacion (0: ok, -1: error)
    info_log("conexion_a_kernel.c@enviar_proceso_nuevo.c", "Esperando respuesta de KERNEL");
    bool response = recv_ack(socket);
    format_debug_log("conexion_a_kernel.c@enviar_proceso_nuevo", "Respuesta de kernel recibida: %d", response);

    if (response) {
      info_log("conexion_a_kernel.c@enviar_proceso_nuevo", "Respuesta de kernel recibida: proceso terminado con exito");
    } else {
      info_log("conexion_a_kernel.c@enviar_proceso_nuevo", "Respuesta de kernel recibida: proceso terminado con errores");
    }
    return response;
  }

  error_log("conexion_a_kernel.c@enviar_proceso_nuevo", "Error al enviar mensaje");

  return 0;
}

int conectar_con_servidor() {
  int socket = crear_conexion(consola_config->ip_kernel, consola_config->puerto_kernel, "Kernel",
                              "conexion_a_kernel.c@conectar_con_servidor");

  if (socket == -1) {
    error_log("conexion_a_kernel.c@conectar_con_servidor",
              "Error al conectar con el servidor de kernel, verificar ip:puerto "
              "y que el servidor este corriendo");
  }

  return socket;
}

void enviar_proceso_a_kernel(t_list *lista_de_instrucciones, uint32_t tamanio_proceso) {
  op_code_t codop = OPCODE_NUEVO_PROCESO;
  void *stream;
  
  debug_log("conexion_a_kernel.c@enviar_proceso_a_kernel", "Serializando proceso a enviar al kernel");
  uint32_t tamanio_stream = serializar_lista_de_instrucciones(lista_de_instrucciones, codop, &stream, tamanio_proceso);


	format_debug_log("serializacion.c@serializar_lista_de_instrucciones", "Tamaño del proceso: %d", tamanio_proceso); 
	
	int socket = conectar_con_servidor();

	if (socket != -1) {
    info_log("conexion_a_kernel.c@enviar_proceso_a_kernel", "Conexion con KERNEL establecida, enviando proceso nuevo");
		enviar_proceso_nuevo(socket, stream, tamanio_stream);
	}

	free(stream);
}
