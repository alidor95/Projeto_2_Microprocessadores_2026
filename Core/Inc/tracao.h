#ifndef TRACAO_H
#define TRACAO_H

#include "main.h" 
#include <stdint.h>

// Protótipos das funções do motor (O Músculo)
void Inicializar_Tracao(void);
void Definir_Pwm_Tracao(uint8_t porcentagem);
uint8_t Obter_Pwm_Tracao(void);

#endif 