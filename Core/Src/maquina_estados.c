/*#include "maquina_estados.h"
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
//Chamado continuamente pelo while(1) do main
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
*/
#include "maquina_estados.h"
#include "central_dados.h"
#include "tracao.h"
#include <stdbool.h> 

static uint8_t estado_atual;
static bool aceleracao_longa = false; 

void Inicializar_Maquina_Estados(void) {
    estado_atual = ESTADO_DESLIGADO;
    display_modo_atual = DISPLAY_MODO_RPM;
    
    // Limpezas de segurança no boot
    flags_sistema &= ~FLAG_AUTOMATICO_ATIVO; 
    aceleracao_longa = false;
}

uint8_t Obter_Estado_Atual(void) {
    return estado_atual;
}

void Atualizar_Maquina_Estados(void) {
    
    uint8_t pwm_agora = Obter_Pwm_Tracao();

    // =========================================================================
    // 0. AÇÕES PARALELAS (Monitoramento de Temperatura)
    // =========================================================================
    if (temperatura_celsius >= 50) { 
        // Acima de 50: Mantém o LED de falha aceso direto
        HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_SET);
    } 
    else if (temperatura_celsius >= 40) { 
        // Entre 40 e 49: Pisca o LED a 1 Hz (500ms ON / 500ms OFF)
        // Como o tick_base é 50ms, 20 ticks formam 1 segundo.
        if ((tick_base % 20) < 10) {
            HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_RESET);
        }
    } 
    else {
        // Abaixo de 40: Apaga o LED
        HAL_GPIO_WritePin(LED_TEMP_GPIO_Port, LED_TEMP_Pin, GPIO_PIN_RESET);
    }

    // =========================================================================
    // 1. INTERTRAVAMENTO DE SEGURANÇA (OVERRIDE GLOBAL)
    // =========================================================================
    
    // A. EMERGÊNCIA (Prioridade 1 - Apenas no Automático)
    if (flags_sistema & FLAG_OBSTACULO_DETECTADO) {
        // O sensor ultrassom SÓ é operante se o carro estiver no Automático
        if (estado_atual == ESTADO_AUTOMATICO) {
            estado_atual = ESTADO_EMERGENCIA;
            flags_sistema &= ~FLAG_AUTOMATICO_ATIVO; 
            tick_emergencia = 0; // Prepara a rampa de freio
        }
    }
    // B. SUPERAQUECIMENTO (Prioridade 2 - Inércia Forçada)
    else if (temperatura_celsius >= 50) {
        // Se estiver tracionando, corta a força e deixa na Inércia
        if (estado_atual == ESTADO_ACELERANDO || estado_atual == ESTADO_AUTOMATICO) {
            estado_atual = ESTADO_INERCIA;
            flags_sistema &= ~FLAG_AUTOMATICO_ATIVO; 
            display_modo_atual = DISPLAY_MODO_FALHA_AQUEC;
        }
    }

    // =========================================================================
    // 2. MÁQUINA DE ESTADOS PRINCIPAL
    // =========================================================================
    switch (estado_atual) {

        case ESTADO_DESLIGADO:
            aceleracao_longa = false; 
            
            if (pwm_agora > 0) {
                Definir_Pwm_Tracao(0);
            }
            HAL_GPIO_WritePin(RELE_GPIO_Port, RELE_Pin, GPIO_PIN_RESET);
            flags_sistema &= ~FLAG_RELE_MOTOR_LIGADO;

            if ((flags_sistema & FLAG_ACELERADOR_PRESSIONADO) && (tick_acelerador >= TICKS_PEDAL_PRESSIONADO)) {
                // Impede a partida se a temperatura estiver crítica (Acelerador inoperante >= 50)
                if (temperatura_celsius < 50) {
                    HAL_GPIO_WritePin(RELE_GPIO_Port, RELE_Pin, GPIO_PIN_SET);
                    flags_sistema |= FLAG_RELE_MOTOR_LIGADO;
                    Definir_Pwm_Tracao(PWM_INICIAL_PARTIDA); 
                    estado_atual = ESTADO_ACELERANDO;
                    
                    aceleracao_longa = true; 
                    tick_acelerador = 0;
                }
            }
            break;

        case ESTADO_INERCIA:
            if (flags_sistema & FLAG_FREIO_PRESSIONADO) {
                estado_atual = ESTADO_FREANDO;
            } 
            // Só permite voltar a acelerar se o motor não estiver fervendo
            else if ((flags_sistema & FLAG_ACELERADOR_PRESSIONADO) && (temperatura_celsius < 50)) {
                aceleracao_longa = false; 
                estado_atual = ESTADO_ACELERANDO;
            } 
            else if (flags_sistema & FLAG_AUTOMATICO_ATIVO) {
                estado_atual = ESTADO_AUTOMATICO;
            } 
            else if (tick_inercia >= TICKS_DECAIMENTO_INERCIA) { 
                if (pwm_agora >= PWM_MINIMO_RELE + PWM_DECREMENTO_INERCIA) {
                    Definir_Pwm_Tracao(pwm_agora - PWM_DECREMENTO_INERCIA);
                    tick_inercia = 0;
                } else {
                    estado_atual = ESTADO_DESLIGADO;
                }
            }
            break;

        case ESTADO_ACELERANDO:
            if (!(flags_sistema & FLAG_ACELERADOR_PRESSIONADO)) {
                if (!aceleracao_longa && tick_acelerador > 0 && tick_acelerador < TICKS_PULSO_RAPIDO_MAX) {
                    flags_sistema |= FLAG_AUTOMATICO_ATIVO;
                    estado_atual = ESTADO_AUTOMATICO;
                } else {
                    estado_atual = ESTADO_INERCIA;
                }
                tick_acelerador = 0; 
            } 
            else if (tick_acelerador >= TICKS_PEDAL_PRESSIONADO) { 
                aceleracao_longa = true; 

                uint8_t teto_pwm = 100;
                if (potenciometro_adc > (VOLANTE_CENTRO + VOLANTE_ZONA_MORTA) || 
                    potenciometro_adc < (VOLANTE_CENTRO - VOLANTE_ZONA_MORTA)) { 
                    teto_pwm = 75; 
                }

                if (pwm_agora + PWM_INCREMENTO_ACEL <= teto_pwm) {
                    Definir_Pwm_Tracao(pwm_agora + PWM_INCREMENTO_ACEL);
                } else {
                    Definir_Pwm_Tracao(teto_pwm); 
                }
                tick_acelerador = 0; 
            }
            break;

        case ESTADO_FREANDO:
            if (!(flags_sistema & FLAG_FREIO_PRESSIONADO)) {
                estado_atual = ESTADO_INERCIA;
            } 
            else if (tick_freio >= TICKS_PEDAL_PRESSIONADO) { 
                if (pwm_agora >= PWM_MINIMO_RELE + PWM_DECREMENTO_FREIO) {
                    Definir_Pwm_Tracao(pwm_agora - PWM_DECREMENTO_FREIO);
                    tick_freio = 0;
                } else {
                    estado_atual = ESTADO_DESLIGADO;
                }
            }
            break;

        case ESTADO_AUTOMATICO:
            if ((flags_sistema & FLAG_FREIO_PRESSIONADO) || (flags_sistema & FLAG_ACELERADOR_PRESSIONADO)) {
                flags_sistema &= ~FLAG_AUTOMATICO_ATIVO; 
                estado_atual = ESTADO_INERCIA; 
            }
            break;

        case ESTADO_EMERGENCIA:
            // Rampa de freio focada da Emergência (-20% a cada 0.5s)
            if (tick_emergencia >= TICKS_PEDAL_PRESSIONADO) { 
                if (pwm_agora >= PWM_MINIMO_RELE + PWM_DECREMENTO_EMERGENCIA) {
                    Definir_Pwm_Tracao(pwm_agora - PWM_DECREMENTO_EMERGENCIA);
                    tick_emergencia = 0;
                } else {
                    estado_atual = ESTADO_DESLIGADO; 
                }
            }
            break;
            
        case ESTADO_AQUECIMENTO:
            // Este estado mantive ativo caso precise retornar o modelo anterior,
            // mas a nova lógica do intertravamento vai redirecionar a falha 
            // de temperatura direto para a Inércia.
            break;
    }
}