
#ifndef CENTRAL_DADOS_H
#define CENTRAL_DADOS_H

#include "main.h"   /* Necessário para os tipos HAL e defines de pinos      */
#include <stdint.h> /* uint8_t, uint16_t, uint32_t                          */


#define FLAG_AUTOMATICO_ATIVO        (1U << 0)  
#define FLAG_RELE_MOTOR_LIGADO     (1U << 1)  
#define FLAG_ACELERADOR_PRESSIONADO (1U << 2) 
#define FLAG_FREIO_PRESSIONADO     (1U << 3)  

/* Bits 4-7: Alertas de temperatura */
#define FLAG_ALERTA_TEMP_AMARELO   (1U << 4) 
#define FLAG_ALERTA_TEMP_CRITICO   (1U << 5)  
#define FLAG_SUPERAQUECIMENTO_CRONICO (1U << 6) 

/* Bits 8-11: Sinalização e painel */
#define FLAG_SETA_ATIVA            (1U << 8)  
#define FLAG_PAINEL_AUTOMATICO       (1U << 9)  

/* Bits 12-15: Obstáculo (ultrassom) */
#define FLAG_OBSTACULO_DETECTADO   (1U << 12) 

extern volatile uint32_t flags_sistema;

extern volatile uint8_t estado_tracao_atual;

extern volatile uint16_t rpm_motor;

extern volatile uint8_t velocidade_kmh;

extern volatile uint32_t odometro_cm;

extern volatile uint16_t temperatura_adc;

extern volatile int16_t temperatura_celsius;

extern volatile uint16_t potenciometro_adc;

extern volatile uint32_t tick_base;

extern volatile uint16_t tick_acelerador;

extern volatile uint16_t tick_freio;

extern volatile uint16_t tick_inercia;

extern volatile uint16_t tick_emergencia;

extern volatile uint16_t tick_temp_alta;

extern volatile uint32_t tick_temp_critica;

extern volatile uint16_t tick_seta;

//#define DISPLAY_MODO_VELOCIDADE    0  
#define DISPLAY_MODO_RPM           0  
#define DISPLAY_MODO_ODOMETRO      1  
#define DISPLAY_MODO_TEMPERATURA   2  
#define DISPLAY_MODO_FALHA_AQUEC   3  

extern volatile uint8_t display_modo_atual;

extern volatile uint8_t display_buffer[3];

extern volatile uint8_t display_digito_atual;

extern volatile uint16_t volante_angulo_anterior;

extern volatile uint16_t tick_volante_movimento;

extern volatile uint16_t ultrasson_distancia_cm;

#define ULTRASSON_THRESHOLD_CM     30U

#define TICK_PERIODO_MS            50U

#define PWM_MINIMO_RELE            19U

#define PWM_INICIAL_PARTIDA        20U

#define PWM_INCREMENTO_ACEL        4U

#define PWM_DECREMENTO_FREIO       20U

#define PWM_DECREMENTO_INERCIA     4U

#define PWM_DECREMENTO_EMERGENCIA  20U

#define RPM_MAXIMO                 1980U

#define VELOCIDADE_MAXIMA_KMH      120U

#define PERIMETRO_RODA_CM          195U

#define TEMP_ALERTA_AMARELO        40

#define TEMP_ALERTA_CRITICO        50

#define TICKS_CONFIRMA_TEMP_ALTA   2U

#define TICKS_SUPERAQUECIMENTO     2400U

#define TICKS_PEDAL_PRESSIONADO    10U

#define TICKS_DECAIMENTO_INERCIA   20U

#define TICKS_PULSO_RAPIDO_MAX     6U

#define VELOCIDADE_MAXIMA_CURVA    90U

#endif 


