#include "../include/main.h"

int main(int argc, char* argv[]) {
    puts("Hello world!!");

    iniciar_config_mem_swap("./mem_swap.config");
    cerrar_mem_swap();

    return 0;
}
