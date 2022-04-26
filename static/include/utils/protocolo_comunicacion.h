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

  /*Consola a Kernel*/
  NUEVO_PROCESO,   
  /*Kernel a CPU*/
  EJECUTAR,/*Mensaje de Kernel a CPU */

  /*Kernel a Memoria*/
  READ,   /*Mensaje de CPU a memoria */
  WRITE, /**Mensaje de CPU a memoria */
  COPY,/*Mensaje de CPU a memoria*/ 
  VALUE_TAB_PAG, /*Mensaje de valor de la tabla a Kernel*/
  CERRAR_PROCESO, //TODO CONSULTAR QUIEN LO ENVIA
  SUSPENDER_PROCESO, /*Mensaje de suspension de proceso enviado por Kernel a Memoria*/
  PING_PONG_MEMORIA,   /*Mensaje de CPU a memoria pidiendo datos -> cant de entradas + tam de pagina */
  ACCESO_1ER_NIVEL, /*Mensaje de CPU a memoria pidiendo datos -> se trata de una tabla de primer nivel, el número de la tabla de segundo nivel. */
  ACCESO_2DO_NIVEL  /*Mensaje de CPU a memoria pidiendo datos -> Si se trata de una tabla de segundo nivel, el número de marco correspondiente. */
 
}op_code_t;


bool send_ack(int socket, op_code_t resultado);
bool recv_ack(int socket);
bool send_prueba(int socket);
bool send_codigo_op(int socket, op_code_t op_code);

#endif /* STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_ */
