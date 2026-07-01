#include "central_dados.h"

volatile uint32_t flags_sistema             = 0U;

volatile uint8_t marcha_atual = 1;

volatile uint16_t rpm_motor                 = 0U;
volatile uint8_t  velocidade_kmh            = 0U;
volatile uint32_t hodometro_cm              = 0U;
volatile uint16_t temperatura_adc           = 0U;
volatile int16_t  temperatura_celsius       = 0;
volatile uint16_t potenciometro_adc         = 0U;

volatile uint32_t tick_base                 = 0U;
volatile uint16_t tick_acelerador           = 0U;
volatile uint16_t tick_freio                = 0U;
volatile uint16_t tick_inercia              = 0U;
volatile uint16_t tick_emergencia           = 0U;
volatile uint16_t volante_centro_calibrado = 2048U;
volatile uint16_t tick_temp_alta            = 0U;
volatile uint32_t tick_temp_critica         = 0U;
volatile uint16_t tick_seta                 = 0U;
volatile uint8_t  flag_tick_50ms            = 0U;


volatile uint16_t adc_dma_buffer[2]         = {0U, 0U};

volatile uint8_t  display_modo_atual        = DISPLAY_MODO_VELOCIDADE;
volatile uint8_t  display_buffer[3]         = {0U, 0U, 0U};
volatile uint8_t  display_digito_atual      = 0U;

volatile uint16_t volante_angulo_anterior   = 0U;
volatile uint16_t tick_volante_movimento    = 0U;

volatile uint16_t ultrasson_distancia_cm    = 9999U;