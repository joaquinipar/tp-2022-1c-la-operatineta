#include "../include/dispatcher.h"

int socket_cliente_cpu_dispatch;
pthread_t thread_escucha;

void escucha_proceso_desalojado();
void lanzar_thread_escucha_proceso_desalojado();

int iniciar_conexion_cpu_dispatch()
{
    debug_log("dispatcher.c.c@iniciar_conexion_cpu_dispatch", "Se inicia conexion dispatcher con CPU");

    socket_cliente_cpu_dispatch = crear_conexion(kernel_config->ip_cpu, kernel_config->puerto_cpu_dispatch, "Kernel", "dispatcher.c@iniciar_conexion_cpu_dispatch");

    if (socket_cliente_cpu_dispatch == -1)
    {
        return 0;
    }

    return 1;
}

void enviar_proceso_a_cpu(pcb_t *proceso) {
    format_info_log("dispatcher.c@enviar_proceso_a_cpu", "Enviando proceso: %d a CPU", proceso->pid);
    send_codigo_op(socket_cliente_cpu_dispatch, OPCODE_PRUEBA_EJECUTAR);
    lanzar_thread_escucha_proceso_desalojado();
}

void lanzar_thread_escucha_proceso_desalojado()
{
    debug_log("dispatcher.c@lanzar_thread_escucha_proceso_desalojado", "Lanzando thread escucha dispatch");
    pthread_create(&thread_escucha, NULL, (void *)escucha_proceso_desalojado, NULL);
    pthread_detach(thread_escucha);
}

void escucha_proceso_desalojado()
{
    debug_log("dispatcher.c@lanzar_thread_escucha_proceso_desalojado", "Escuchando desalojo de CPU en dispatch");

    op_code_t codigo_operacion;
    if (recv(socket_cliente_cpu_dispatch, &codigo_operacion, sizeof(op_code_t), 0) !=
        sizeof(op_code_t))
    {
        error_log("dispatcher.c@escucha_proceso_desalojado", "Se recibio un codigo_operacion invalido!");
    }

    format_debug_log("dispatcher.c@escucha_proceso_desalojado", "CodOp recibido: %d", codigo_operacion);

    // TODO: informar qué proceso volvió de la CPU
    info_log("dispatcher.c@escucha_proceso_desalojado", "CPU nos mando un proceso de regreso");
}