#ifndef MAQUINA_ESTADOS_H
#define MAQUINA_ESTADOS_H

#include "main.h"
#include <stdint.h>

// Definição dos 7 Estados da FSM
#define ESTADO_DESLIGADO   0
#define ESTADO_INERCIA     1
#define ESTADO_ACELERANDO  2
#define ESTADO_FREANDO     3
#define ESTADO_AUTOMATICO  4
#define ESTADO_EMERGENCIA  5

// Protótipos das funções
void Inicializar_Maquina_Estados(void);
uint8_t Obter_Estado_Atual(void);
void Atualizar_Maquina_Estados(void);

#endif