#include <utils/protocolo_comunicacion.h>


bool send_ack(int socket, op_code_t resultado) {
  if (send(socket, &resultado, sizeof(op_code_t), 0) == -1) {
    return false;
  }
  return true;
}

bool recv_ack(int socket) {
  op_code_t resultado;
  if ((recv(socket, &resultado, sizeof(op_code_t), 0) == sizeof(op_code_t)) &&
      (resultado == ACK_ERROR)) {
    return false;
  }
  return true;
}

bool send_prueba(int socket) {
  op_code_t cop = PRUEBA;
  if (send(socket, &cop, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
    return false;
  }
  return true;
}


