
#include "../../include/botonera/botonera_helper.h"

int desconectar_cliente(int sockett) {
    uint32_t cliente_desconect = (uint32_t) CLIENTE_DESCONECTADO;
    send(sockett, &cliente_desconect, sizeof(uint32_t), 0);
}
