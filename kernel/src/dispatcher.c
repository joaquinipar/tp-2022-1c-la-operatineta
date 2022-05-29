#include "../include/dispatcher.h"

int socket_cliente_cpu_dispatch;
pthread_t thread_escucha;

void escucha_proceso_desalojado();
void lanzar_thread_escucha_proceso_desalojado();

int iniciar_conexion_cpu_dispatch()
{
    debug_log("dispatcher.c@iniciar_conexion_cpu_dispatch", "Se inicia conexion dispatcher con CPU");

    if (kernel_config->is_test == 1) {
        debug_log("dispatcher.c@iniciar_conexion_cpu_dispatch", "IS_TEST = 1, simulando");
        return 1;
    }

    socket_cliente_cpu_dispatch = crear_conexion(kernel_config->ip_cpu, kernel_config->puerto_cpu_dispatch, "Kernel", "dispatcher.c@iniciar_conexion_cpu_dispatch");

    if (socket_cliente_cpu_dispatch == -1)
    {
        return 0;
    }

    return 1;
}

void enviar_proceso_a_cpu(pcb_t *proceso) {
    format_info_log("dispatcher.c@enviar_proceso_a_cpu", "Enviando proceso: %d a CPU", proceso->pid);

    if (kernel_config->is_test) {
        format_debug_log("dispatcher.c@enviar_proceso_a_cpu", "se esta testeando, no se envia nada");
        return;
    }

    void *stream_proceso;
    uint32_t cant_bytes_serializados = serializar_proceso(proceso, OPCODE_EJECUTAR, stream_proceso);
    send(socket_cliente_cpu_dispatch, stream_proceso, cant_bytes_serializados, 0);

    lanzar_thread_escucha_proceso_desalojado();
}

void lanzar_thread_escucha_proceso_desalojado()
{
    debug_log("dispatcher.c@lanzar_thread_escucha_proceso_desalojado", "Lanzando thread escucha dispatch");
    pthread_create(&thread_escucha, NULL, (void *)escucha_proceso_desalojado, NULL);
    pthread_detach(thread_escucha);
}


// TODO: traer los mensajes de server_kernel.c de lo que hizo Bel
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

    switch (codigo_operacion) {

        case OPCODE_PROCESO_DESALOJADO_IO: {

            format_debug_log("dispatcher.c@procesar_conexion", "CodOP recibido %d", codigo_operacion);
            pcb_t* proceso_actualizado = deserializar_proceso(socket_cliente_cpu_dispatch);
            uint32_t tiempo_bloqueo;
            int bytes_recibidos = recv(socket_cliente_cpu_dispatch, &tiempo_bloqueo, sizeof(uint32_t), 0); 
            format_debug_log("dispatcher.c@procesar_conexion", "Tiempo bloqueado %d", tiempo_bloqueo); 
                if (bytes_recibidos !=-1) {
            send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_OK); 
            debug_log("conexion_client.c@procesar_conexion", "Envio mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_IO");
            debug_log("conexion_client.c@procesar_conexion", "Termina mensaje OPCODE_PROCESO_DESALOJADO_IO");
            }
            send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_ERROR); 
            debug_log("conexion_client.c@procesar_conexion", "Envio mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_IO");
            debug_log("conexion_client.c@procesar_conexion", "Termina mensaje OPCODE_PROCESO_DESALOJADO_IO");

        }

        case OPCODE_PROCESO_DESALOJADO_INTERRUPT: {

            format_debug_log("dispatcher.c@procesar_conexion", "CodOP recibido %d", codigo_operacion);
            pcb_t* proceso_actualizado = deserializar_proceso(socket_cliente_cpu_dispatch); 
            if(proceso_actualizado != NULL){
            send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_OK); 
            debug_log("conexion_client.c@procesar_conexion", "Envio mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_INTERRUPT");
            debug_log("conexion_client.c@procesar_conexion", "Termina mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
            }
            send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_ERROR); 
            debug_log("conexion_client.c@procesar_conexion", "Envio mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_EXIT");
            debug_log("conexion_client.c@procesar_conexion", "Termina mensaje OPCODE_PROCESO_DESALOJADO_EXIT");
            
        }

        case OPCODE_PROCESO_DESALOJADO_EXIT: {

            format_debug_log("dispatcher.c@procesar_conexion", "CodOP recibido %d", codigo_operacion);
            pcb_t* proceso_actualizado = deserializar_proceso(socket_cliente_cpu_dispatch); 

            if(proceso_actualizado != NULL){
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_OK); 
                debug_log("conexion_client.c@procesar_conexion", "Envio mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_EXIT");
                debug_log("conexion_client.c@procesar_conexion", "Termina mensaje OPCODE_PROCESO_DESALOJADO_EXIT");
            }

            send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_ERROR); 
            debug_log("conexion_client.c@procesar_conexion", "Envio mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_INTERRUPT");
            debug_log("conexion_client.c@procesar_conexion", "Termina mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
            
        }
        
        default:
            break;

    }

    // TODO: informar qué proceso volvió de la CPU
    info_log("dispatcher.c@escucha_proceso_desalojado", "CPU nos mando un proceso de regreso");

    
}