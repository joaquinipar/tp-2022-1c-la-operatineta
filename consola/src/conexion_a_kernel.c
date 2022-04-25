#include "../include/conexion_a_kernel.h"

bool enviar_proceso_nuevo(int socket, void* stream, uint32_t tamanio_stream) {
  format_info_log("conexion_a_kernel.c@enviar_proceso_nuevo", "tamanio de stream: %d", tamanio_stream);

  int send_result = send(socket, stream, tamanio_stream, false);

  

  format_info_log("conexion_a_kernel.c@enviar_proceso_nuevo", "tamanio send result: %d", send_result);


  if (send_result != -1) {
    // Esperar a que el kernel responda por la inicializacion (0: ok, -1: error)
    bool response = recv_ack(socket);

    debug_log("conexion_a_kernel.c@enviar_proceso_nuevo",
              "Respuesta de kernel recibida");

    return response;
  }

  error_log("conexion_a_kernel.c@enviar_proceso_nuevo",
            "Error al enviar mensaje");

  return -1;

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

void enviar_proceso_a_kernel(t_list *lista_de_instrucciones) {
    //SERIALIZAR
	uint32_t codop = (uint32_t) NUEVO_PROCESO; // TODO
	void *stream;
	uint32_t tamanio_stream = serializar_lista_de_instrucciones(lista_de_instrucciones, codop, &stream);

	int socket = conectar_con_servidor();

    format_debug_log("conexion_a_kernel.c@enviar_proceso_a_kernel", "socket obtenido: %d", socket);

	if (socket != -1) {
    debug_log("conexion_a_kernel.c@enviar_proceso_a_kernel", "Servidora conectada, enviando proceso...");
		enviar_proceso_nuevo(socket, stream, tamanio_stream);
	}

	free(stream);
}
