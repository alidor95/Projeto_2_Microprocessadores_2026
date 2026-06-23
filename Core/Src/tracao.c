#include "tracao.h"

extern TIM_HandleTypeDef htim1;

static uint8_t pwm_porcentagem_atual;

void Inicializar_Tracao(void) {
    pwm_porcentagem_atual = 0;
    Definir_Pwm_Tracao(0); 
}

void Definir_Pwm_Tracao(uint8_t porcentagem) {
    if (porcentagem > 100) {
        porcentagem = 100;
    }
    pwm_porcentagem_atual = porcentagem;
    
    uint32_t valor_registrador = (porcentagem * 3999) / 100;
    
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, valor_registrador);
}

uint8_t Obter_Pwm_Tracao(void) {
    return pwm_porcentagem_atual;
}
