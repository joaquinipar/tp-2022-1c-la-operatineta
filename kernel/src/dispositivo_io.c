#include "../include/dispositivo_io.h"

sem_t sem_cont_procesos_bloqueados;
pthread_t thread_dispositivo_io;
int ejecutando = 1;

int escuchar_peticiones_dispositivo();

void iniciar_dispositivo_io() {
    sem_init(&sem_cont_procesos_bloqueados, 0, 0);

    pthread_create(&thread_dispositivo_io, NULL, (void *)escuchar_peticiones_dispositivo, NULL);
    pthread_detach(thread_dispositivo_io);
}

void incrementar_cantidad_procesos_bloqueados() {
  sem_post(&sem_cont_procesos_bloqueados);
}

int escuchar_peticiones_dispositivo() {

  format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Escuchando peticiones para el dispositivo IO");

  while (ejecutando) {

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Esperando call_io para dispositivo IO");
    sem_wait(&sem_cont_procesos_bloqueados);

    bloqueo_proceso_t *proceso_bloqueado = desencolar_proceso_bloqueado();
    pcb_t *proceso = proceso_bloqueado->proceso;
    int retardo = proceso_bloqueado->retardo;

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Llamada a IO recibida para dispositivo IO. Esperando %d ms para el proceso: %d", retardo, proceso->pid);

    usleep(retardo * 1000);

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "El proceso: %d ya cumplio sus %d ms, mover a suspended-ready o ready dependiendo su estado", proceso->pid, retardo);

    io_dispositivo_finalizada(proceso_bloqueado->proceso);

  }

  return 0;
}