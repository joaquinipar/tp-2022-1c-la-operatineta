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
    info_log("-","-----------------------------------------------------------------------------------------------------"); 
    socket_cliente_cpu_dispatch = crear_conexion(kernel_config->ip_cpu, kernel_config->puerto_cpu_dispatch, "Dispatch", "dispatcher.c@iniciar_conexion_cpu_dispatch");

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

    enviar_mensaje_ejecutar(proceso);
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

    switch (codigo_operacion) {

        case OPCODE_PROCESO_DESALOJADO_IO: {

            format_debug_log("dispatcher.c@escucha_proceso_desalojado", "CodOP recibido %d", codigo_operacion);

            uint32_t tiempo_bloqueo;
            int bytes_recibidos = recv(socket_cliente_cpu_dispatch, &tiempo_bloqueo, sizeof(uint32_t), 0); 
            format_debug_log("dispatcher.c@escucha_proceso_desalojado", "Tiempo bloqueado %d", tiempo_bloqueo);

            // TODO: creo que no hay ningun free para este proceso_actualizado, hacer despues de llamar a la funcion que corresponde
            pcb_t* proceso_actualizado = deserializar_proceso(socket_cliente_cpu_dispatch);
            
            if (bytes_recibidos !=-1) {
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_OK); 
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Envio mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_IO");
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Termina mensaje OPCODE_PROCESO_DESALOJADO_IO");
            } else {
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_ERROR); 
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Envio mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_IO");
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Termina mensaje OPCODE_PROCESO_DESALOJADO_IO");
            }

            bloquear_proceso(proceso_actualizado, tiempo_bloqueo);
            break;

        }

        case OPCODE_PROCESO_DESALOJADO_INTERRUPT: {

            format_debug_log("dispatcher.c@escucha_proceso_desalojado", "CodOP recibido %d", codigo_operacion);

            // TODO: creo que no hay ningun free para este proceso_actualizado, hacer despues de llamar a la funcion que corresponde
            pcb_t* proceso_actualizado = deserializar_proceso(socket_cliente_cpu_dispatch); 

            if(proceso_actualizado != NULL){
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_OK); 
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Envio mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_INTERRUPT");
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Termina mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
            } else {
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_ERROR); 
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Envio mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_INTERRUPT");
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Termina mensaje OPCODE_PROCESO_DESALOJADO_INTERRUPT");
            }

            desalojar_proceso_interrupt(proceso_actualizado);

            break;
            
        }

        case OPCODE_PROCESO_DESALOJADO_EXIT: {

            format_debug_log("dispatcher.c@escucha_proceso_desalojado", "CodOP recibido %d", codigo_operacion);
            pcb_t* proceso_actualizado = deserializar_proceso(socket_cliente_cpu_dispatch); 

            if(proceso_actualizado != NULL){
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_OK); 
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Envio mensaje ACK OK - OPCODE_PROCESO_DESALOJADO_EXIT");
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Termina mensaje OPCODE_PROCESO_DESALOJADO_EXIT");
            } else {
                send_ack(socket_cliente_cpu_dispatch, OPCODE_ACK_ERROR); 
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Envio mensaje ACK ERROR -OPCODE_PROCESO_DESALOJADO_EXIT");
                debug_log("conexion_client.c@escucha_proceso_desalojado", "Termina mensaje OPCODE_PROCESO_DESALOJADO_EXIT");
            }

            finalizar_proceso(proceso_actualizado);

            break;

        }
        
        default:
            break;

    }    
}