#include "maquina_estados.h"

static uint8_t estado_atual;

void Inicializar_Maquina_Estados(void) {

    estado_atual = ESTADO_DESLIGADO;
}

uint8_t Obter_Estado_Atual(void) {
    return estado_atual;
}