#ifndef STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_
#define STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

typedef enum op_code_t {

  OPCODE_CLIENTE_DESCONECTADO = -1,
  OPCODE_ACK_OK,
  OPCODE_ACK_ERROR,
  OPCODE_PRUEBA,
  OPCODE_PRUEBA_EJECUTAR, // dummy para probar kernel -> cpu -> kernel

  /*Consola a Kernel*/
  OPCODE_NUEVO_PROCESO,

  /*Kernel a CPU*/
  OPCODE_EJECUTAR,/*Mensaje de Kernel a CPU */
  OPCODE_DESALOJAR_PROCESO, // Enviado por interrupt para que cpu desaloje un proceso

  /* CPU a Kernel*/
  OPCODE_PROCESO_DESALOJADO, // Este codigo se manda por el dispatch a Kernel indicando que la CPU detuvo su ejecucion (el estado refleja a que cola debe ir: ready, bloqueado, exit)

  /*Kernel a Memoria*/
  OPCODE_READ,   /*Mensaje de CPU a memoria */ 
  OPCODE_WRITE, /**Mensaje de CPU a memoria */
  OPCODE_VALUE_TAB_PAG, /*Mensaje de valor de la tabla a Kernel*/  
  OPCODE_CERRAR_PROCESO, //TODO CONSULTAR QUIEN LO ENVIA
  OPCODE_SUSPENDER_PROCESO, /*Mensaje de suspension de proceso enviado por Kernel a Memoria*/
  OPCODE_PING_PONG_MEMORIA,   /*Mensaje de CPU a memoria pidiendo datos -> cant de entradas + tam de pagina */
  OPCODE_ACCESO_1ER_NIVEL, /*Mensaje de CPU a memoria pidiendo datos -> se trata de una tabla de primer nivel, el número de la tabla de segundo nivel. */
  OPCODE_ACCESO_2DO_NIVEL,  /*Mensaje de CPU a memoria pidiendo datos -> Si se trata de una tabla de segundo nivel, el número de marco correspondiente. */

  OPCODE_EXIT

} op_code_t;

bool send_ack(int socket, op_code_t resultado);
bool recv_ack(int socket);
bool send_prueba(int socket);
bool send_codigo_op(int socket, op_code_t op_code);

/**
 * @name: send_codigo_op_con_numero
 * @desc: Envia un [ CODOP | NUMERO ]
 * @param socket
 * @param cop
 * @param numero
 * @return bool
 */
bool send_codigo_op_con_numero(int socket, op_code_t cop, uint32_t numero);

/**
 * @name: send_codigo_op_con_numeros
 * @desc: Envia un [ CODOP | NUMERO1 | NUMERO2 ]
 * @param socket
 * @param cop
 * @param numero1
 * @param numero2
 * @return bool
 */
bool send_codigo_op_con_numeros(int socket, op_code_t cop, uint32_t numero1, uint32_t numero2);

#endif /* STATIC_INCLUDE_UTILS_PROTOCOLO_COMUNICACION_H_ */
