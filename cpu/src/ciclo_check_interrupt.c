#include "../include/ciclo_check_interrupt.h"

int interrupt_flag = 0; 

void encender_flag_interrupt(){
    interrupt_flag = 1;
}

bool chequear_interrupt(){
    return interrupt_flag == 1;
}

void apagar_flag_interrupt(){
    interrupt_flag = 0;
}

/* manejo_de_interrupt()

if chequear interrupt 
        enviar mensaje de desalojo a kernel
 else
        continuar ejecutando */