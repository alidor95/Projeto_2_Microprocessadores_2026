#include "tracao.h"
#include "central_dados.h"
extern TIM_HandleTypeDef htim1;

static uint8_t pwm_porcentagem_atual;

void Inicializar_Tracao(void) {
    pwm_porcentagem_atual = 0;
    Definir_Pwm_Tracao(0); 
}

void Definir_Pwm_Tracao(uint8_t valor) {
    if (flags_sistema & FLAG_RELE_MOTOR_LIGADO) {
        // Aplica o PWM no hardware
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, valor);
    } else {
        // Se o relé estiver desligado, garante PWM zero
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
    }
    pwm_porcentagem_atual = valor;
}

uint8_t Obter_Pwm_Tracao(void) {
    return pwm_porcentagem_atual;
}
