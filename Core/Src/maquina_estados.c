#include "maquina_estados.h"
#include "central_dados.h"
#include "tracao.h"

static uint8_t estado_atual;

void Inicializar_Maquina_Estados(void) {
    // Inicia o sistema no estado Desligado
    estado_atual = ESTADO_DESLIGADO;
}

uint8_t Obter_Estado_Atual(void) {
    return estado_atual;
}

// O CÉREBRO: Chamado continuamente pelo while(1) do main
void Atualizar_Maquina_Estados(void) {
    
    switch (estado_atual) {
        
        case ESTADO_DESLIGADO:
            
            break;

        case ESTADO_INERCIA:
            // Lógica futura: Decaimento suave de 4% a cada 1s
            break;

        case ESTADO_ACELERANDO:
            // Lógica futura: Incremento de 4% a cada 0.5s
            break;

        case ESTADO_FREANDO:
            // Lógica futura: Decremento de 20% a cada 0.5s
            break;

        case ESTADO_AUTOMATICO:
            // Lógica futura: Travar PWM atual
            break;

        case ESTADO_AQUECIMENTO:
            // Lógica futura: Forçar desligamento por temperatura
            break;

        case ESTADO_EMERGENCIA:
            // Lógica futura: Decremento brusco por obstáculo
            break;
    }
}
