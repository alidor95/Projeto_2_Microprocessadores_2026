# 🏎️ Projeto 2: Microprocessadores (2026) - Computador de Bordo Automotivo

**Equipe Desenvolvedora:** Alisson Dorneles, Eduardo Fioravanti e Tiago Jardim

## 📖 Visão Geral do Projeto
Este repositório contém o firmware de um computador de bordo e sistema de controle (Powertrain e Telemetria) desenvolvido para um microcontrolador **STM32F411CEU6 (Black Pill)**. 

O projeto foi arquitetado com foco em otimização de memória e execução em baixo nível, utilizando a HAL da STMicroelectronics via ambiente **CMake + VS Code**. O sistema gerencia a interface do usuário (display multiplexado), leitura de sensores via DMA e controle de velocidade/RPM através de interrupções de hardware (Input Capture).

---

## 📍 Mapeamento de Hardware (Pinout)

### 📺 Interface e Painel (Display Multiplexado)
A varredura dos displays é controlada pelo `TIM10` para garantir uma atualização fluida e livre de travamentos.
* **PB3 a PB9:** `SEG_A` até `SEG_G` (Segmentos do Display).
* **PB10:** `PONTO_D` (Ponto Decimal).
* **PB12, PB13, PB15:** `SELETOR_1`, `SELETOR_2`, `SELETOR_3` (Transistores de ativação dos dígitos).
* **PB0:** `LED_TEMP` (Alerta de sobreaquecimento).
* **PB1:** `LED_SETAS` (Indicador de direção).
* **PC13:** `LED_PILOTO_AUTO` (LED onboard - Lógica Invertida).

### 🕹️ Comandos do Piloto
Entradas digitais configuradas com resistores de Pull-Up internos para garantir estabilidade de sinal.
* **PA15:** `ACELERADOR`
* **PB14:** `FREIO`
* **PA9:** `FUNCAO`
* **PA10:** `SELECIONA`

### ⚙️ Powertrain (Controle de Motor)
* **PA8:** `MOTOR_PWM` -> Saída PWM (`TIM1_CH1`) para acionamento do motor em alta frequência (Open-Drain).
* **PA0:** `TACOMETRO` -> Entrada de Input Capture (`TIM5_CH1`) com filtro anti-ruído de hardware para medição exata do RPM.
* **PA4:** `RELE` -> Saída digital para corte de segurança.

### 🧠 Sensores e Telemetria
O ADC está configurado em modo Circular via DMA, evitando que o processador perca tempo de clock com leituras analógicas.
* **PA1:** `POTENCIOMETRO` -> Leitura da posição do volante (`ADC1_IN1`).
* **PA3:** `ULTRASSON_GATE` -> Pulso de trigger para o sensor de distância.
* **PA2:** `ULTRASSON_ECHO` -> Retorno do sensor lido via Input Capture (`TIM2_CH3`).
* **Interno:** `Sensor de Temperatura` -> Monitoramento térmico do próprio silício da placa via ADC.

---

## 🚀 Como Compilar o Projeto
Este projeto não utiliza o STM32CubeIDE. Para compilar:
1. Clone o repositório.
2. Abra a pasta no **VS Code** com as extensões oficiais da ST e do CMake instaladas.
3. Configure o Toolchain para `GCC for arm-none-eabi`.
4. Clique em **Build** na barra inferior. Todo o código do usuário está isolado dentro das tags `USER CODE` no arquivo `Core/Src/main.c`.
