#ifndef STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_
#define STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

typedef enum op_code_t {

  CLIENTE_DESCONECTADO = -1,
  ACK_OK,
  ACK_ERROR,
  PRUEBA,

  //CONSOLA A KERNEL
  NUEVO_PROCESO 
}op_code_t;


bool send_ack(int socket, op_code_t resultado);
bool recv_ack(int socket);
bool send_prueba(int socket);
bool send_codigo_op(int socket, op_code_t op_code);

#endif /* STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_ */
