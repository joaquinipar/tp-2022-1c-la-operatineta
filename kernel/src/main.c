#include "config_kernel.h"


int main(int argc, char* argv[]) {

    setup_logger();

    iniciar_config_kernel("../config/ejemplo.config");

    print_config();

    return 0;
}
