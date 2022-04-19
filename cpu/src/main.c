#include "../include/main.h"


int main(int argc, char* argv[]) {
    puts("Hello world!!");

    iniciar_config_cpu("./cpu.config");


    cerrar_cpu();
    return 0;
}
