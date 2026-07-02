#ifndef CENTRAL_DADOS_H
#define CENTRAL_DADOS_H

#include "main.h"
#include <stdint.h>

// --- Bits de Estado do Sistema ---
#define FLAG_AUTOMATICO_ATIVO           (1U << 0)
#define FLAG_RELE_MOTOR_LIGADO          (1U << 1)
#define FLAG_ACELERADOR_PRESSIONADO     (1U << 2)
#define FLAG_FREIO_PRESSIONADO          (1U << 3)
#define FLAG_OBSTACULO_DETECTADO        (1U << 4)

// --- Bits de Alertas e Segurança ---
#define FLAG_ALERTA_TEMP_AMARELO        (1U << 5)
#define FLAG_ALERTA_TEMP_CRITICO        (1U << 6)
#define FLAG_SUPERAQUECIMENTO_CRONICO   (1U << 7)

// --- Bits de Interface e Sinalização ---
#define FLAG_SETA_DIR                   (1U << 8)
#define FLAG_SETA_ESQ                   (1U << 9)
#define FLAG_PAINEL_AUTOMATICO          (1U << 10)


typedef enum
{
    VOLANTE_PARADO = 0,
    VOLANTE_DIREITA,
    VOLANTE_ESQUERDA
} movimento_volante_t;

extern movimento_volante_t movimento_volante;
extern uint8_t tick_volante;

extern volatile uint32_t flags_sistema;
extern volatile uint8_t marcha_atual; // Valores de 1 a 5

extern volatile uint16_t rpm_motor;
extern volatile uint8_t  velocidade_kmh;
extern volatile uint32_t hodometro_cm;
extern volatile uint16_t temperatura_adc;
extern volatile int16_t  temperatura_celsius;
extern volatile uint16_t potenciometro_adc;

extern volatile uint32_t tick_base;
extern volatile uint16_t tick_acelerador;
extern volatile uint16_t tick_freio;
extern volatile uint16_t tick_inercia;
extern volatile uint16_t tick_emergencia;
extern volatile uint16_t tick_temp_alta;
extern volatile uint32_t tick_temp_critica;
extern volatile uint16_t tick_seta;
extern volatile uint8_t  flag_tick_50ms;
extern volatile uint16_t volante_centro_calibrado;

extern volatile uint16_t adc_dma_buffer[2];

#define DISPLAY_MODO_VELOCIDADE     0
#define DISPLAY_MODO_RPM            1
#define DISPLAY_MODO_ODOMETRO       2
#define DISPLAY_MODO_TEMPERATURA    3
#define DISPLAY_MODO_FALHA_AQUEC    4

extern volatile uint8_t display_modo_atual;
extern volatile uint8_t display_buffer[3];
extern volatile uint8_t display_digito_atual;

extern volatile uint16_t volante_angulo_anterior;
extern volatile uint16_t tick_volante_movimento;

extern volatile uint16_t ultrasson_distancia_cm;

#define ULTRASSON_THRESHOLD_CM      30U
#define TICK_PERIODO_MS             50U
#define PWM_MINIMO_RELE             19U
#define PWM_INICIAL_PARTIDA         20U
#define PWM_INCREMENTO_ACEL         4U
#define PWM_DECREMENTO_FREIO        20U
#define PWM_DECREMENTO_INERCIA      4U
#define PWM_DECREMENTO_EMERGENCIA   20U
#define RPM_MAXIMO                  1980U
#define VELOCIDADE_MAXIMA_KMH       120U
#define PERIMETRO_RODA_CM           195U
#define TEMP_ALERTA_AMARELO         40
#define TEMP_ALERTA_CRITICO         50
#define TICKS_CONFIRMA_TEMP_ALTA    2U
#define TICKS_SUPERAQUECIMENTO      2400U
#define TICKS_PEDAL_PRESSIONADO     10U
#define TICKS_DECAIMENTO_INERCIA    20U
#define TICKS_PULSO_RAPIDO_MAX      6U
#define VELOCIDADE_MAXIMA_CURVA     90U
#define VOLANTE_CENTRO              2048U
#define VOLANTE_ZONA_MORTA          500U

#endif