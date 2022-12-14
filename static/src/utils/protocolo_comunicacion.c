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
      (resultado == OPCODE_ACK_ERROR)) {
    return false;
  }
  return true;
}

bool send_prueba(int socket) {
  op_code_t cop = OPCODE_PRUEBA;
  if (send(socket, &cop, sizeof(op_code_t), 0) != sizeof(op_code_t)) {
    return false;
  }
  return true;
}

bool send_codigo_op(int socket, op_code_t cop) {
  size_t size = sizeof(op_code_t);
  return send(socket, &cop, size, 0) != -1;
}


bool send_codigo_op_con_numero(int socket, op_code_t cop, uint32_t numero) {
    size_t size = sizeof (uint32_t) + sizeof (numero);
    void* serializacion = malloc(size);
    uint32_t codop_uint = (uint32_t) cop; // para no enviar tipos dependientes del sistema (o sea int)
    memcpy(serializacion, &codop_uint, sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t), &numero , sizeof (numero));

    int res = send(socket, serializacion, size, 0);
    free(serializacion);
    return res != -1;
}

bool send_codigo_op_con_numeros(int socket, op_code_t cop, uint32_t numero1, uint32_t numero2) {
    size_t size = sizeof (uint32_t) + sizeof (uint32_t) + sizeof (uint32_t);
    void* serializacion = malloc(size);
    uint32_t codop_uint = (uint32_t) cop; // para no enviar tipos dependientes del sistema (o sea int)
    memcpy(serializacion, &codop_uint, sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t), &numero1 , sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t) + sizeof (uint32_t), &numero2 , sizeof (uint32_t));
    int res = send(socket, serializacion, size, 0);
    free(serializacion);
    return  res != -1;
}

bool send_codigo_op_con_3_numeros(int socket, op_code_t cop, uint32_t numero1, uint32_t numero2, uint32_t numero3) {

    size_t size = sizeof (uint32_t) + sizeof (uint32_t) + sizeof (uint32_t) + sizeof (uint32_t);
    void* serializacion = malloc(size);
    uint32_t codop_uint = (uint32_t) cop; // para no enviar tipos dependientes del sistema (o sea int)
    memcpy(serializacion, &codop_uint, sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t), &numero1 , sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t) + sizeof (uint32_t), &numero2 , sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t) + sizeof (uint32_t) + sizeof (uint32_t), &numero3 , sizeof (uint32_t));

    int res = send(socket, serializacion, size, 0);
    free(serializacion);
    return res != -1;
}

bool send_codigo_op_con_contenido(int socket, op_code_t cop, uint32_t contenido) {
    size_t size = sizeof (uint32_t) + sizeof (uint32_t);
    void* serializacion = malloc(size);
    uint32_t codop_uint = (uint32_t) cop; // para no enviar tipos dependientes del sistema (o sea int)
    memcpy(serializacion, &codop_uint, sizeof (uint32_t));
    memcpy(serializacion + sizeof (uint32_t), &contenido , sizeof (uint32_t));

    int res = send(socket, serializacion, size, 0);
    free(serializacion);
    return res != -1;
}