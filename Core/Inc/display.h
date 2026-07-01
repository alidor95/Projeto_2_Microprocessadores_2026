#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"
#include <stdint.h>

typedef enum {
    MODO_NUMERICO,
    MODO_MENSAGEM_AQU,
    MODO_MENSAGEM_47C,
    MODO_PILOTO_AUTO // Adicionado para a Opção A
} DisplayModo_t;

void Display_Init(void);
void Display_Atualizar(void);
void Display_SetValor(uint16_t valor);
void Display_SetModo(DisplayModo_t modo);

#endif