# Controle de Servo Motor e LED com Raspberry Pi Pico

Este repositório contém um código em C para controlar um servo motor e um LED utilizando um Raspberry Pi Pico. O código foi desenvolvido para demonstrar o funcionamento básico de um servo motor, que é controlado por sinais PWM (Pulse Width Modulation), e também para explorar o comportamento de um LED quando submetido a sinais PWM.

Aqui, você encontrará uma explicação detalhada do código, instruções sobre como executá-lo e uma análise do comportamento do LED quando substitui o servo motor.

---
# Cálculos de Frequência e Ciclo Ativo para PWM (Servomotor)

## 1. Cálculo de `freqInt`  
A fórmula para a frequência do PWM é:  

$$
F_{pwm} = \frac{F_{init}}{(freqInt + \frac{FreqFac}{16}) \cdot wrap}
$$

Substituindo os valores fornecidos:  

- $$( F_{pwm} = 50 \ Hz $$)
- $$( F_{init} = 125 \ MHz = 125 \times 10^6 $$)
- $$( FreqFac = 0 $$)
- $$( wrap = 65535 $$)

A fórmula simplifica para:  

$$
50 = \frac{125 \times 10^6}{freqInt \cdot 65535}
$$


1. Multiplicamos $$( 50 \times 65535 = 3.27675 \times 10^6 $$)
2. Dividimos $$( 125 \times 10^6 \) por \( 3.27675 \times 10^6 $$):


$$
freqInt \approx 38,1
$$

---

## 2. Cálculo do Ciclo Ativo (Duty Cycle)  
O período de um sinal PWM com frequência de **50 Hz** é:

$$
T = \frac{1}{F_{PWM}} = \frac{1}{50} = 20 \, ms = 20.000 \, \mu s
$$

O ciclo ativo (Duty Cycle) é a razão entre o tempo ativo ($( t_{on} $)) e o período total ($( T $)):

$${Duty Cycle} = \frac{t_{on}}{T} \times 100 $$

### Cálculos:

1. **Tempo ativo de 2.400 µs → Ciclo Ativo: 12%**  
   $${Duty Cycle} = \frac{2400}{20000} \times 100 = 12 $$ 

2. **Tempo ativo de 1.470 µs → Ciclo Ativo: 7,35%**  
   $${Duty Cycle} = \frac{1470}{20000} \times 100 = 7,35 $$ 

3. **Tempo ativo de 500 µs → Ciclo Ativo: 2,5%**  
   $${Duty Cycle} = \frac{500}{20000} \times 100 = 2,5 $$

---

## 3. Conversão para Valor PWM (Wrap de 65535)  
O valor correspondente no contador PWM é proporcional ao duty cycle:

$$({Valor PWM} = \text{Duty Cycle} \times 65535) $$

### Cálculos:

1. **Ciclo Ativo de 12% (2.400 µs)**  
   $${Valor PWM} = 0,12 \times 65535 = 7864$$

2. **Ciclo Ativo de 7,35% (1.470 µs)**  
   $$
   {Valor PWM} = 0,0735 \times 65535 = 4817
   $$

3. **Ciclo Ativo de 2,5% (500 µs)**  
   $$
   {Valor PWM} = 0,025 \times 65535 = 1638
   $$

---

## Resumo dos Resultados:

| Tempo Ativo (\( \mu s \)) | Ciclo Ativo (%) | Valor PWM |
|---------------------------|-----------------|-----------|
| 2.400                      | 12%             | 7864      |
| 1.470                      | 7,35%           | 4817      |
| 500                        | 2,5%            | 1638      |

---

## Observação:
Esses valores podem ser usados para controlar a posição de um servomotor, ajustando o pulso para diferentes ângulos:
- **2.400 µs → 180 graus**
- **1.470 µs → 90 graus**
- **500 µs → 0 graus**

---

## Descrição do Código

O código é composto por várias funções que configuram e controlam o servo motor e o LED. Abaixo está uma descrição detalhada de cada parte do código:

### Inclusão de Bibliotecas

```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdlib.h>
```

- **`stdio.h`**: Biblioteca padrão de entrada e saída.
- **`pico/stdlib.h`**: Biblioteca específica do Raspberry Pi Pico para configuração e controle de GPIO.
- **`hardware/pwm.h`**: Biblioteca para controle de PWM no Raspberry Pi Pico.
- **`stdlib.h`**: Biblioteca padrão para funções utilitárias.

---

### Definição de Constantes

```c
const uint pino_servo = 22; // Pino GPIO conectado ao servo motor
const uint pino_led = 11;   // Pino GPIO conectado ao LED

const uint zero_graus = 1638;          // 2,5% de 65535 -> 1638 (0°)
const uint noventa_graus = 4817;       // 7,35% de 65535 -> 4817 (90°)
const uint cento_oitenta_graus = 7864; // 12% de 65535 -> 7864 (180°)

const float frequencia_pwm = 38.1f; // Frequência de 50 Hz
const uint wrap_pwm = 65535;        // Valor máximo do contador PWM (65535 passos)
```

- **`pino_servo`**: Pino GPIO conectado ao servo motor.
- **`pino_led`**: Pino GPIO conectado ao LED.
- **`zero_graus`, `noventa_graus`, `cento_oitenta_graus`**: Valores de duty cycle correspondentes a 0°, 90° e 180° do servo motor.
- **`frequencia_pwm`**: Frequência do sinal PWM (50 Hz).
- **`wrap_pwm`**: Valor máximo do contador PWM (65535).

---

### Funções

#### `setup_led()`

```c
void setup_led() {
    gpio_init(pino_led);
    gpio_set_dir(pino_led, GPIO_OUT);
    gpio_put(pino_led, 0);
}
```

- **Descrição**: Configura o pino do LED como saída e inicializa o LED desligado.

---

#### `setup_pwm(uint pino)`

```c
void setup_pwm(uint pino) {
    gpio_set_function(pino, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pino);
    pwm_set_clkdiv(slice, frequencia_pwm);
    pwm_set_wrap(slice, wrap_pwm);
    pwm_set_gpio_level(pino, zero_graus);
    pwm_set_enabled(slice, true);
}
```

- **Descrição**: Configura o pino especificado para funcionar como PWM. Define a frequência, o valor de wrap e o duty cycle inicial.

---

#### `girar_servo_180(uint pino)`

```c
void girar_servo_180(uint pino) {
    pwm_set_gpio_level(pino, cento_oitenta_graus);
    sleep_ms(5000);
}
```

- **Descrição**: Move o servo motor para 180° e aguarda 5 segundos.

---

#### `girar_servo_90(uint pino)`

```c
void girar_servo_90(uint pino) {
    pwm_set_gpio_level(pino, noventa_graus);
    sleep_ms(5000);
}
```

- **Descrição**: Move o servo motor para 90° e aguarda 5 segundos.

---

#### `girar_servo_0(uint pino)`

```c
void girar_servo_0(uint pino) {
    pwm_set_gpio_level(pino, zero_graus);
    sleep_ms(5000);
}
```

- **Descrição**: Move o servo motor para 0° e aguarda 5 segundos.

---

#### `mover_servo_suavemente_decrescente(uint pino)`

```c
void mover_servo_suavemente_decrescente(uint pino) {
    for (uint i = cento_oitenta_graus; i >= zero_graus; i -= 20) {
        pwm_set_gpio_level(pino, i);
        sleep_ms(10);
    }
}
```

- **Descrição**: Move o servo motor suavemente de 180° para 0°.

---

#### `mover_servo_suavemente_crescente(uint pino)`

```c
void mover_servo_suavemente_crescente(uint pino) {
    for (uint i = zero_graus; i <= cento_oitenta_graus; i += 20) {
        pwm_set_gpio_level(pino, i);
        sleep_ms(10);
    }
}
```

- **Descrição**: Move o servo motor suavemente de 0° para 180°.

---

### Função Principal (`main`)

```c
int main() {
    stdio_init_all(); // Inicializa a comunicação serial

    setup_led(); // Configura o LED

    setup_pwm(pino_servo); // Configura o PWM para o servo
    setup_pwm(pino_led);   // Configura o PWM para o LED

    while (true) {
        girar_servo_180(pino_servo);
        girar_servo_90(pino_servo);
        girar_servo_0(pino_servo);
        mover_servo_suavemente_crescente(pino_servo);
        mover_servo_suavemente_decrescente(pino_servo);
    }
}
```

- **Descrição**: Inicializa o sistema, configura os pinos e entra em um loop infinito que move o servo motor entre diferentes ângulos e realiza movimentos suaves.

---

## Comportamento do LED no Lugar do Servo

Se o LED for conectado no lugar do servo motor, o comportamento será diferente, pois o LED responde ao sinal PWM de maneira distinta. Aqui está o que acontece:

1. **Duty Cycle Baixo (2,5% - 0°)**: O LED estará quase desligado, emitindo uma luz muito fraca.
2. **Duty Cycle Médio (7,35% - 90°)**: O LED estará parcialmente aceso, emitindo uma luz moderada.
3. **Duty Cycle Alto (12% - 180°)**: O LED estará quase totalmente aceso, emitindo uma luz brilhante.

### Movimento Suave

- **Crescente**: O LED aumentará gradualmente o brilho de 0° a 180°.
- **Decrescente**: O LED diminuirá gradualmente o brilho de 180° a 0°.

### Observações

- O LED não tem a capacidade de "mover" como um servo motor, mas sim de variar sua intensidade luminosa conforme o duty cycle do sinal PWM.
- O comportamento do LED é linear em relação ao duty cycle, enquanto o servo motor responde a pulsos específicos para posicionamento angular.

---

## Como Executar o Código

### Pré-requisitos

1. **Hardware**:
   - Raspberry Pi Pico.
   - Servo motor (ou LED, se quiser testar o comportamento do LED).
   - Jumpers e protoboard (opcional, para facilitar as conexões).

2. **Software**:
   - Ambiente de desenvolvimento configurado para o Raspberry Pi Pico (recomendamos o [Raspberry Pi Pico SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)).
   - Compilador GCC para ARM (já incluído no SDK).

### Passos para Execução

1. **Conectar o Servo Motor ou LED**:
   - Conecte o servo motor ao pino GPIO 22.
   - Conecte o LED ao pino GPIO 11 (com um resistor limitador de corrente, se necessário).

2. **Configurar o Ambiente**:
   - Certifique-se de que o Raspberry Pi Pico SDK está instalado e configurado.
   - Clone este repositório ou copie o código para o seu ambiente de desenvolvimento.

3. **Compilar o Código**:
   - No terminal, navegue até o diretório do projeto e execute:
     ```bash
     mkdir build
     cd build
     cmake ..
     make
     ```

4. **Carregar o Código no Pico**:
   - Conecte o Raspberry Pi Pico ao computador via USB.
   - Pressione o botão `BOOTSEL` no Pico enquanto o conecta para colocá-lo no modo de armazenamento em massa.
   - Copie o arquivo `.uf2` gerado na pasta `build` para o Pico.

5. **Executar**:
   - O código será executado automaticamente. Observe o servo motor movendo-se entre 0°, 90° e 180°, ou o LED variando sua intensidade.

---

## Conclusão

Este código é uma excelente demonstração de como controlar dispositivos como servo motores e LEDs usando PWM no Raspberry Pi Pico. Ele ilustra a versatilidade do Pico em aplicações de controle e automação, permitindo desde o posicionamento preciso de um servo motor até a variação de intensidade de um LED.

Sinta-se à vontade para modificar e expandir o código para atender às suas necessidades! Se tiver dúvidas ou sugestões, abra uma issue neste repositório. 😊

---
