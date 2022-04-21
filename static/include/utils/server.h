#ifndef INCLUDE_UTILS_SERVER_H_
#define INCLUDE_UTILS_SERVER_H_

#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utils/logger.h>

int iniciar_servidor(char *ubicacion, const char *nameCliente, char *ip, char *puerto);
int esperar_cliente(int socket_servidor, char *nameServer, char *ubicacion);
int crear_conexion(char *ip, char *puerto, char *nameServer, char *ubicacion);
void liberar_conexion(int *socket_cliente, char *ubicacion);

#endif /* INCLUDE_UTILS_SERVER_H_ */
