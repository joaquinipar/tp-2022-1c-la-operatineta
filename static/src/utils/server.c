#include <utils/server.h>

int iniciar_servidor(char *ubicacion, const char *nameCliente, char *ip,char *puerto) {

 // debug_log("server.c@iniciar_servidor", "Inicializando socket del servidor");
  int socket_servidor;

  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(ip, puerto, &hints, &servinfo);

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((socket_servidor =
             socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
      continue;

    if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
      close(socket_servidor);
      continue;
    }
    break;
  }

  listen(socket_servidor, SOMAXCONN);
 // debug_log("server.c@iniciar_servidor", "Servidor listo para recibir al cliente");

  freeaddrinfo(servinfo);

  return socket_servidor;
}

int esperar_cliente(int socket_servidor, char *nameServer, char *ubicacion) {
  debug_log("static/server.c@esperar_cliente", "Esperando cliente nuevo");
  struct sockaddr_in dir_cliente;
  socklen_t tam_direccion = sizeof(struct sockaddr_in);

  int socket_cliente = accept(socket_servidor, (void *)&dir_cliente, &tam_direccion);

  format_debug_log("static/server.c@esperar_cliente", "Cliente conectado a Servidor %s", nameServer);

  return socket_cliente;
}

int crear_conexion(char *ip, char *puerto, char *nameServer, char *ubicacion) {

  struct addrinfo hints;
  struct addrinfo *servinfo;

  // Init de hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Recibe addrinfo
  getaddrinfo(ip, puerto, &hints, &servinfo);

  // Crea un socket con la informacion recibida (del primero, suficiente)
  int socket_cliente =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  // Fallo en crear el socket
  if (socket_cliente == -1) {
    char *mensaje =
        string_from_format("Error creando el socket para %s:%s\n", ip, puerto);
    error_log(ubicacion, mensaje);
    free(mensaje);
    return -1;
  }

  // Error conectando
  if (connect(socket_cliente, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    // errorLog(ubicacion,  "Error al conectar (a %s)\n", nameServer);
    char *mensaje = string_from_format("Error al conectar a %s\n", nameServer);
    error_log(ubicacion, mensaje);
    free(mensaje);
    freeaddrinfo(servinfo);
    return -1;
  } else {
    char *mensaje = string_from_format(
        "Cliente conectado en %s:%s a Servidor %s\n", ip, puerto, nameServer);
    info_log(ubicacion, mensaje);
    free(mensaje);
  }

  freeaddrinfo(servinfo);

  return socket_cliente;
}

void liberar_conexion(int *socket_cliente, char *ubicacion) {
  info_log(ubicacion, "Se libera conexion");
  close(*socket_cliente);
  *socket_cliente = -1;
}