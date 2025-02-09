#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

const uint pino_servo = 22;
const uint pino_led = 11;

const uint zero_graus = 1638;  // 2,5% de 65535 -> 1638
const uint noventa_graus = 4817;  // 7,35% de 65535 -> 4817
const uint cento_oitenta_graus = 7864;  // 12% de 65535 -> 7864

const float frequencia_pwm = 38.1f;  // Frequência de 50 Hz
const uint wrap_pwm = 65535;  // 65535 passos

void setup_led(); // Protótipo da função para configurar o pino do LED
void setup_pwm(uint pino, uint *slice); // Protótipo da função para configurar o pino do servo com frequência de 50 Hz
void girar_servo_180(uint pino, uint slice); // Protótipo da função para girar o servo para 180 graus (Duty Cycle de 12%)
void girar_servo_90(uint pino, uint slice); // Protótipo da função para girar o servo para 90 graus (Duty Cycle de 7,35%)
void girar_servo_0(uint pino, uint slice); // Protótipo da função para girar o servo para 0 graus (Duty Cycle de 2,5%)
void mover_servo_suavemente(uint pino, uint slice, uint inicio, uint fim); // Protótipo da função para mover o servo suavemente de um ângulo para outro

int main() {
    stdio_init_all();

    setup_led();

    uint slice;
    setup_pwm(pino_servo, &slice);

    while (true) {

        girar_servo_180(pino_servo, slice); // 0° -> 180°
        girar_servo_90(pino_servo, slice); // 180° -> 90°
        girar_servo_0(pino_servo, slice); // 90° -> 0° 
        
        mover_servo_suavemente(pino_servo, slice, zero_graus, cento_oitenta_graus);  // 0° -> 180°
        mover_servo_suavemente(pino_servo, slice, cento_oitenta_graus, zero_graus);  // 180° -> 0°

        sleep_ms(1000);  // Atraso de 1 segundo
    }
}

/*
 * Função para configurar o pino do LED
 */
void setup_led() {
    gpio_init(pino_led);
    gpio_set_dir(pino_led, GPIO_OUT);
    gpio_put(pino_led, 0);
}


/*
 * Função para configurar o pino do servo com frequência de 50 Hz
 */
void setup_pwm(uint pino, uint *slice) {
    gpio_set_function(pino, GPIO_FUNC_PWM);  // Habilitar o pino GPIO como PWM
    *slice = pwm_gpio_to_slice_num(pino);    // Obter o canal PWM da GPIO
    pwm_set_clkdiv(*slice, frequencia_pwm);           // Define o divisor de clock do PWM
    pwm_set_wrap(*slice, wrap_pwm);              // Define o valor de wrap (4000 passos)
}


/*
 * Função para girar o servo para 180 graus (Duty Cycle de 12%)
 */
void girar_servo_180(uint pino, uint slice) {
    pwm_set_gpio_level(pino, cento_oitenta_graus);  // 12% de 65535 -> 7864
    pwm_set_enabled(slice, true);   // Habilita o PWM no slice correspondente
    sleep_ms(5000);                 // Aguarda 5 segundos
}


/*
 * Função para girar o servo para 90 graus (Duty Cycle de 7,35%)
 */
void girar_servo_90(uint pino, uint slice) {
    pwm_set_gpio_level(pino, noventa_graus); // 7,35% de 65535 -> 4817  
    pwm_set_enabled(slice, true);   // Habilita o PWM no slice correspondente
    sleep_ms(5000);                 // Aguarda 5 segundos
}


/*
 * Função para girar o servo para 0 graus (Duty Cycle de 2,5%)
 */
void girar_servo_0(uint pino, uint slice) {
    pwm_set_gpio_level(pino, zero_graus);  // 2,5% de 65535 -> 1638
    pwm_set_enabled(slice, true);   // Habilita o PWM no slice correspondente
    sleep_ms(5000);                 // Aguarda 5 segundos
}


/*
 * Função para mover o servo suavemente de um ângulo para outro
 */
void mover_servo_suavemente(uint pino, uint slice, uint inicio, uint fim) {
    int passo = (inicio < fim) ? 16 : -16;
    for (int nivel = inicio; nivel != fim; nivel += passo) {
        pwm_set_gpio_level(pino, nivel);
        sleep_ms(10);  // Atraso de 10 ms para suavizar o movimento
    }
    pwm_set_gpio_level(pino, fim);  // Garante que chegue ao valor final
}
