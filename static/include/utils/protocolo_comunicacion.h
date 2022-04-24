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
  PRUEBA
}op_code_t;


bool send_ack(int socket, op_code_t resultado);
bool recv_ack(int socket);

#endif /* STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_ */
