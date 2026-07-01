#include "display.h"

// Tabela de conversão para display de catodo comum
static const uint8_t digitos[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static uint16_t valor_exibicao = 0;
static DisplayModo_t modo_atual = MODO_NUMERICO;

void Display_Init(void) {
    // Configuração inicial conforme necessário
}

void Display_SetValor(uint16_t valor) {
    if (valor > 999) {
        valor_exibicao = 999;
    } else {
        valor_exibicao = valor;
    }
}

void Display_SetModo(DisplayModo_t modo) {
    modo_atual = modo;
}

void Display_Atualizar(void) {
    static uint8_t digito_ativo = 0;
    uint8_t valor = 0;
    uint8_t segmentos = 0;

    // Lógica de seleção de modo e extração de dígitos
    if (modo_atual == MODO_NUMERICO) {
        if (digito_ativo == 0) {
            valor = (valor_exibicao / 100) % 10;
        } else if (digito_ativo == 1) {
            valor = (valor_exibicao / 10) % 10;
        } else {
            valor = valor_exibicao % 10;
        }
        segmentos = digitos[valor];
    } 
    else if (modo_atual == MODO_MENSAGEM_AQU) {
        if (digito_ativo == 0) {
            segmentos = 0x77; // A
        } else if (digito_ativo == 1) {
            segmentos = 0x67; // q
        } else {
            segmentos = 0x3E; // u
        }
    }
    else if (modo_atual == MODO_MENSAGEM_47C) {
        if (digito_ativo == 0) {
            segmentos = 0x66; // 4
        } else if (digito_ativo == 1) {
            segmentos = 0x07; // 7
        } else {
            segmentos = 0x39; // C
        }
    }
    else if (modo_atual == MODO_PILOTO_AUTO) {
        if (digito_ativo == 0) {
            segmentos = 0x73; // P
        } else if (digito_ativo == 1) {
            segmentos = 0x77; // A
        } else {
            segmentos = 0x00; // Desligado
        }
    }

    // Desliga todos os seletores antes de aplicar os estados
    HAL_GPIO_WritePin(GPIOB, SELETOR_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SELETOR_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, SELETOR_3_Pin, GPIO_PIN_RESET);

    // Escreve nos segmentos individualmente
    if (segmentos & 0x01) {
        HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, GPIO_PIN_RESET);
    }

    if (segmentos & 0x02) {
        HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, GPIO_PIN_RESET);
    }

    if (segmentos & 0x04) {
        HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, GPIO_PIN_RESET);
    }

    if (segmentos & 0x08) {
        HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, GPIO_PIN_RESET);
    }

    if (segmentos & 0x10) {
        HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, GPIO_PIN_RESET);
    }

    if (segmentos & 0x20) {
        HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, GPIO_PIN_RESET);
    }

    if (segmentos & 0x40) {
        HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, GPIO_PIN_RESET);
    }

    // Liga seletor do dígito ativo
    if (digito_ativo == 0) {
        HAL_GPIO_WritePin(SELETOR_1_GPIO_Port, SELETOR_1_Pin, GPIO_PIN_SET);
    } else if (digito_ativo == 1) {
        HAL_GPIO_WritePin(SELETOR_2_GPIO_Port, SELETOR_2_Pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(SELETOR_3_GPIO_Port, SELETOR_3_Pin, GPIO_PIN_SET);
    }

    // Passa para o próximo dígito
    digito_ativo = digito_ativo + 1;
    if (digito_ativo > 2) {
        digito_ativo = 0;
    }
}