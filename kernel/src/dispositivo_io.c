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

int escuchar_peticiones_dispositivo() {

  format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Escuchando peticiones para el dispositivo IO");

  while (ejecutando) {

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Esperando call_io para dispositivo IO");

    sem_wait(&sem_cont_procesos_bloqueados);

    bloqueo_proceso_t *proceso_bloqueado = desencolar_proceso_bloqueado();
    pcb_t *proceso = proceso_bloqueado->proceso;
    int tiempo_bloqueo = proceso_bloqueado->retardo;

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Llamada a IO recibida para dispositivo IO. Esperando %d ms para el proceso: %d", proceso_bloqueado->retardo, proceso_bloqueado->proceso->pid);

    int tiempo_suspended_ready = 0;
    if(kernel_config->tiempo_maximo_bloqueado < tiempo_bloqueo){
        tiempo_suspended_ready = tiempo_bloqueo - kernel_config->tiempo_maximo_bloqueado;
        usleep(kernel_config->tiempo_maximo_bloqueado);

        pcb_t *proceso_desencolado_de_bloqueado = desencolar_proceso_bloqueado_IO(proceso);
        encolar_proceso_en_bloqueados_suspendidos(proceso_desencolado_de_bloqueado);
        enviar_mensaje_suspender_proceso(proceso_desencolado_de_bloqueado);
        usleep(tiempo_suspended_ready);

        pcb_t *proceso_desencolado_de_suspended_ready = desencolar_proceso_bloqueado_suspendido_IO(proceso_desencolado_de_bloqueado);
        encolar_proceso_en_suspendidos_listos(proceso_desencolado_de_suspended_ready);

        sem_post(&sem_proceso_listo);   // llamar a planificar largo plazo

    } else {
        usleep(tiempo_bloqueo);
    }

    //sem_post(&sem_bin_procesar_listo);   // llamar a planificar corto plazo

    //usleep(proceso_bloqueado->retardo * 1000);

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Llamada a IO completada para dispositivo IO y proceso: %d", proceso_bloqueado->proceso->pid);

    io_dispositivo_finalizada(proceso_bloqueado->proceso);

    format_debug_log("dispositivos_io.c@escuchar_peticiones_dispositivo", "Proceso: %d agregado a otra cola", proceso_bloqueado->proceso->pid);

  }

  return 0;
}