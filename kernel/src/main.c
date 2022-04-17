#include "config_kernel.h"


int main(int argc, char* argv[]) {
    puts("Hello world!!");
    iniciar_config_kernel("../config/ejemplo.config");

    print_config();



    return 0;
}
