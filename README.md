O seu texto está excelente, Alisson! O mapeamento de hardware está detalhado, técnico e com uma formatação muito profissional. Dá para ver claramente a arquitetura do projeto.

No entanto, há uma armadilha perigosa no finalzinho do seu texto. A sua seção **"Como Compilar o Projeto"** não reflete a realidade da infraestrutura que está rodando na sua máquina. Se o seu professor (ou seus colegas) tentarem seguir esses passos puramente pelo VS Code, eles vão bater de frente com o mesmo erro de *"No compiler found"* que travou o projeto mais cedo, pois dependerão de variáveis de ambiente do Windows estarem perfeitamente configuradas.

Para garantir que a documentação reflita o fluxo de trabalho real, seguro e profissional que valida o código, eu reescrevi a última seção descrevendo a **arquitetura híbrida** (VS Code para edição + CubeIDE para compilação). Fiz também pequenos ajustes de fluidez no restante do texto.

Aqui está a versão corrigida e pronta para o seu repositório:

---

# 🏎️ Projeto 2: Microprocessadores (2026) - Computador de Bordo Automotivo

**Equipe Desenvolvedora:** Alisson Dorneles, Eduardo Fioravanti e Tiago Jardim

## 📖 Visão Geral do Projeto

Este repositório contém o firmware de um computador de bordo e sistema de controle (Powertrain e Telemetria) desenvolvido para o microcontrolador **STM32F411CEU6 (Black Pill)**.

O projeto foi arquitetado com foco em otimização de memória e execução em baixo nível, utilizando a HAL da STMicroelectronics. O sistema gerencia a interface do usuário (display multiplexado), a leitura de sensores via DMA e o controle de velocidade/RPM através de interrupções de hardware (Input Capture).

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

Entradas digitais configuradas com resistores de Pull-Up internos para garantir a estabilidade do sinal.

* **PA15:** `ACELERADOR`
* **PB14:** `FREIO`
* **PA9:** `FUNCAO`
* **PA10:** `SELECIONA`

### ⚙️ Powertrain (Controle de Motor)

* **PA8:** `MOTOR_PWM` -> Saída PWM (`TIM1_CH1`) para acionamento do motor em alta frequência (Open-Drain).
* **PA0:** `TACOMETRO` -> Entrada de Input Capture (`TIM5_CH1`) com filtro anti-ruído de hardware para medição exata do RPM.
* **PA4:** `RELE` -> Saída digital para corte de segurança.

### 🧠 Sensores e Telemetria

O ADC está configurado em modo Circular via DMA, evitando que o processador perca ciclos de clock aguardando conversões analógicas.

* **PA1:** `POTENCIOMETRO` -> Leitura da posição do volante (`ADC1_IN1`).
* **PA3:** `ULTRASSON_GATE` -> Pulso de trigger para o sensor de distância.
* **PA2:** `ULTRASSON_ECHO` -> Retorno do sensor lido via Input Capture (`TIM2_CH3`).
* **Interno:** `Sensor de Temperatura` -> Monitoramento térmico do próprio silício da placa via canal ADC dedicado.

---

## 🚀 Fluxo de Desenvolvimento e Compilação

Este projeto utiliza uma abordagem híbrida, combinando a agilidade do **VS Code** com a robustez do **STM32CubeIDE** para garantir a estabilidade da *toolchain* via CMake.

### 1. Edição de Código

1. Clone o repositório.
2. Abra a pasta raiz do projeto no **VS Code**. Todo o desenvolvimento lógico e edição dos arquivos `.c` e `.h` (isolados dentro das tags `USER CODE` em `Core/Src/main.c`) devem ser feitos aqui.

### 2. Compilação e Gravação

Para compilar o código sem erros de dependência de caminhos do compilador GCC:

1. Abra o **STM32CubeIDE**.
2. Vá em `File > Import > STM32 Project > STM32 CMake Project`.
3. Selecione o diretório raiz deste repositório (onde está localizado o `CMakeLists.txt`).
4. Durante a importação, selecione o microcontrolador alvo: **STM32F411CE**.
5. Finalize a importação e utilize as ferramentas nativas da IDE (*Build/Martelo* e *Debug/Inseto*) para compilar o executável `.elf` e gravá-lo na placa.
