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
    flags_sistema &= ~(FLAG_AUTOMATICO_ATIVO | FLAG_SETA_DIR | FLAG_SETA_ESQ); 
    aceleracao_longa = false;
}

uint8_t Obter_Estado_Atual(void) { return estado_atual; }

void Atualizar_Marcha(void) {
    if (rpm_motor < 1000) marcha_atual = 1;
    else if (rpm_motor < 1500) marcha_atual = 2;
    else if (rpm_motor < 2000) marcha_atual = 3;
    else if (rpm_motor < 2500) marcha_atual = 4;
    else marcha_atual = 5;
}

void Atualizar_Volante(void)
{
    switch (movimento_volante)
    {
        case VOLANTE_PARADO:

            if (potenciometro_adc > (VOLANTE_CENTRO + VOLANTE_ZONA_MORTA))
            {
                movimento_volante = VOLANTE_DIREITA;
                tick_volante = 0;
            }
            else if (potenciometro_adc < (VOLANTE_CENTRO - VOLANTE_ZONA_MORTA))
            {
                movimento_volante = VOLANTE_ESQUERDA;
                tick_volante = 0;
            }

            break;

        case VOLANTE_DIREITA:

            tick_volante++;

            /* Voltou para o centro */
            if ((potenciometro_adc >= (VOLANTE_CENTRO - VOLANTE_ZONA_MORTA)) &&
                (potenciometro_adc <= (VOLANTE_CENTRO + VOLANTE_ZONA_MORTA)))
            {
                if (tick_volante <= TICKS_PULSO_RAPIDO_MAX)
                {
                    /* Liga/desliga seta direita */
                    flags_sistema ^= FLAG_SETA_DIR;

                    /* Garante que a outra seta fique desligada */
                    flags_sistema &= ~FLAG_SETA_ESQ;
                }

                movimento_volante = VOLANTE_PARADO;
            }
            else if (tick_volante > TICKS_PULSO_RAPIDO_MAX)
            {
                /* Demorou demais, descarta o movimento */
                movimento_volante = VOLANTE_PARADO;
            }

            break;

        case VOLANTE_ESQUERDA:

            tick_volante++;

            /* Voltou para o centro */
            if ((potenciometro_adc >= (VOLANTE_CENTRO - VOLANTE_ZONA_MORTA)) &&
                (potenciometro_adc <= (VOLANTE_CENTRO + VOLANTE_ZONA_MORTA)))
            {
                if (tick_volante <= TICKS_PULSO_RAPIDO_MAX)
                {
                    /* Liga/desliga seta esquerda */
                    flags_sistema ^= FLAG_SETA_ESQ;

                    /* Garante que a outra seta fique desligada */
                    flags_sistema &= ~FLAG_SETA_DIR;
                }

                movimento_volante = VOLANTE_PARADO;
            }
            else if (tick_volante > TICKS_PULSO_RAPIDO_MAX)
            {
                /* Demorou demais, descarta o movimento */
                movimento_volante = VOLANTE_PARADO;
            }

            break;
    }
}

void Atualizar_Maquina_Estados(void) {
    Atualizar_Marcha();
    Atualizar_Volante();
    uint8_t pwm_agora = Obter_Pwm_Tracao();

    // 1. EMERGÊNCIA (Apenas no modo automático)
    if (flags_sistema & FLAG_OBSTACULO_DETECTADO) {
        if (estado_atual == ESTADO_AUTOMATICO) {
            estado_atual = ESTADO_EMERGENCIA;
            flags_sistema &= ~FLAG_AUTOMATICO_ATIVO; 
            tick_emergencia = 0;
        }
    }
    // 2. SUPERAQUECIMENTO (>50°C - Inércia forçada)
    else if (temperatura_celsius >= 50) {
        if (estado_atual == ESTADO_ACELERANDO || estado_atual == ESTADO_AUTOMATICO) {
            estado_atual = ESTADO_INERCIA;
            flags_sistema &= ~FLAG_AUTOMATICO_ATIVO;
        }
    }

    switch (estado_atual) {
        case ESTADO_DESLIGADO:
            aceleracao_longa = false; 
            if (pwm_agora > 0) Definir_Pwm_Tracao(0);
            HAL_GPIO_WritePin(RELE_GPIO_Port, RELE_Pin, GPIO_PIN_RESET);
            flags_sistema &= ~FLAG_RELE_MOTOR_LIGADO;

            if ((flags_sistema & FLAG_ACELERADOR_PRESSIONADO) && (tick_acelerador >= TICKS_PEDAL_PRESSIONADO)) {
                if (temperatura_celsius < 50) {
                    HAL_GPIO_WritePin(RELE_GPIO_Port, RELE_Pin, GPIO_PIN_SET);
                    flags_sistema |= FLAG_RELE_MOTOR_LIGADO;
                    Definir_Pwm_Tracao(PWM_INICIAL_PARTIDA); 
                    estado_atual = ESTADO_ACELERANDO;
                    aceleracao_longa = true; tick_acelerador = 0;
                }
            }
            break;

        case ESTADO_INERCIA:
            if (flags_sistema & FLAG_FREIO_PRESSIONADO) estado_atual = ESTADO_FREANDO;
            else if ((flags_sistema & FLAG_ACELERADOR_PRESSIONADO) && (temperatura_celsius < 50)) {
                aceleracao_longa = false; estado_atual = ESTADO_ACELERANDO;
            } 
            else if (flags_sistema & FLAG_AUTOMATICO_ATIVO) estado_atual = ESTADO_AUTOMATICO;
            else if (tick_inercia >= TICKS_DECAIMENTO_INERCIA) { 
                if (pwm_agora >= PWM_MINIMO_RELE + PWM_DECREMENTO_INERCIA) {
                    Definir_Pwm_Tracao(pwm_agora - PWM_DECREMENTO_INERCIA);
                    tick_inercia = 0;
                } else estado_atual = ESTADO_DESLIGADO;
            }
            break;

        case ESTADO_ACELERANDO:
            if (!(flags_sistema & FLAG_ACELERADOR_PRESSIONADO)) {
                if (!aceleracao_longa && tick_acelerador > 0 && tick_acelerador < TICKS_PULSO_RAPIDO_MAX) {
                    flags_sistema |= FLAG_AUTOMATICO_ATIVO;
                    estado_atual = ESTADO_AUTOMATICO;
                } else estado_atual = ESTADO_INERCIA;
                tick_acelerador = 0; 
            } 
            else if (tick_acelerador >= TICKS_PEDAL_PRESSIONADO) { 
                aceleracao_longa = true; 
                uint8_t teto_pwm = (potenciometro_adc > (VOLANTE_CENTRO + VOLANTE_ZONA_MORTA) || potenciometro_adc < (VOLANTE_CENTRO - VOLANTE_ZONA_MORTA)) ? 75 : 100;
                if (pwm_agora + PWM_INCREMENTO_ACEL <= teto_pwm) Definir_Pwm_Tracao(pwm_agora + PWM_INCREMENTO_ACEL);
                else Definir_Pwm_Tracao(teto_pwm); 
                tick_acelerador = 0; 
            }
            break;

        case ESTADO_FREANDO:
            if (!(flags_sistema & FLAG_FREIO_PRESSIONADO)) estado_atual = ESTADO_INERCIA;
            else if (tick_freio >= TICKS_PEDAL_PRESSIONADO) { 
                if (pwm_agora >= PWM_MINIMO_RELE + PWM_DECREMENTO_FREIO) {
                    Definir_Pwm_Tracao(pwm_agora - PWM_DECREMENTO_FREIO);
                    tick_freio = 0;
                } else estado_atual = ESTADO_DESLIGADO;
            }
            break;

        case ESTADO_AUTOMATICO:
            if ((flags_sistema & FLAG_FREIO_PRESSIONADO) || (flags_sistema & FLAG_ACELERADOR_PRESSIONADO)) {
                flags_sistema &= ~FLAG_AUTOMATICO_ATIVO; 
                estado_atual = ESTADO_INERCIA; 
            }
            break;

        case ESTADO_EMERGENCIA:
            if (tick_emergencia >= TICKS_PEDAL_PRESSIONADO) { 
                if (pwm_agora >= PWM_MINIMO_RELE + PWM_DECREMENTO_EMERGENCIA) {
                    Definir_Pwm_Tracao(pwm_agora - PWM_DECREMENTO_EMERGENCIA);
                    tick_emergencia = 0;
                } else estado_atual = ESTADO_DESLIGADO;
            }
            break;
    }
}