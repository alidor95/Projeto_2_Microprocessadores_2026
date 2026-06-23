#ifndef MAQUINA_ESTADOS_H
#define MAQUINA_ESTADOS_H

#include "main.h"
#include <stdint.h>


#define ESTADO_DESLIGADO   0
#define ESTADO_INERCIA     1
#define ESTADO_ACELERANDO  2
#define ESTADO_FREANDO     3
#define ESTADO_CRUZEIRO    4
#define ESTADO_EMERGENCIA  5


void Inicializar_Maquina_Estados(void);
uint8_t Obter_Estado_Atual(void);

#endif 