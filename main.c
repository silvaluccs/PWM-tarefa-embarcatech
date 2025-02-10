#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdlib.h>

const uint pino_servo = 22;
const uint pino_led = 11;

const uint zero_graus = 1638;  // 2,5% de 65535 -> 1638
const uint noventa_graus = 4817;  // 7,35% de 65535 -> 4817
const uint cento_oitenta_graus = 7864;  // 12% de 65535 -> 7864


const float frequencia_pwm = 38.1f;  // Frequência de 50 Hz
const uint wrap_pwm = 65535;  // 65535 passos


void setup_led(); // Protótipo da função para configurar o pino do LED
void setup_pwm(uint pino); // Protótipo da função para configurar o pino do servo com frequência de 50 Hz
void girar_servo_180(uint pino); // Protótipo da função para girar o servo para 180 graus (Duty Cycle de 12%)
void girar_servo_90(uint pino); // Protótipo da função para girar o servo para 90 graus (Duty Cycle de 7,35%)
void girar_servo_0(uint pino); // Protótipo da função para girar o servo para 0 graus (Duty Cycle de 2,5%)
void mover_servo_suavemente_decrescente(uint pino); // Protótipo da função para mover o servo suavemente de um ângulo para outro
void mover_servo_suavemente_crescente(uint pino); // Protótipo da função para mover o servo suavemente de um ângulo para outro


int main() {
    stdio_init_all();

    setup_led();

    setup_pwm(pino_servo);
    setup_pwm(pino_led);

    while (true) {
        girar_servo_180(pino_servo);
        girar_servo_90(pino_servo);
        girar_servo_0(pino_servo);
        mover_servo_suavemente_crescente(pino_servo);
        mover_servo_suavemente_decrescente(pino_servo);
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
void setup_pwm(uint pino) {
    gpio_set_function(pino, GPIO_FUNC_PWM);  // Habilitar o pino GPIO como PWM = pwm_gpio_to_slice_num(pino);    // Obter o canal PWM da GPIO
    uint slice = pwm_gpio_to_slice_num(pino);  // Obter o canal PWM da GPIO
    pwm_set_clkdiv(slice, frequencia_pwm);           // Define o divisor de clock do PWM
    pwm_set_wrap(slice, wrap_pwm);              // Define o valor de wrap (0 passos)
    pwm_set_gpio_level(pino, zero_graus);       // Define o Duty Cycle inicial (2,5%)
    pwm_set_enabled(slice, true);               // Habilita o canal PWM
}


/*
 * Função para girar o servo para 180 graus (Duty Cycle de 12%)
 */
void girar_servo_180(uint pino) {
    pwm_set_gpio_level(pino, cento_oitenta_graus);  // 12% de 65535 -> 7864
    sleep_ms(5000);                 // Aguarda 5 segundos
}


/*
 * Função para girar o servo para 90 graus (Duty Cycle de 7,35%)
 */
void girar_servo_90(uint pino) {
    pwm_set_gpio_level(pino, noventa_graus); // 7,35% de 65535 -> 4817  
    sleep_ms(5000);                 // Aguarda 5 segundos
}


/*
 * Função para girar o servo para 0 graus (Duty Cycle de 2,5%)
 */
void girar_servo_0(uint pino) {
    pwm_set_gpio_level(pino, zero_graus);  // 2,5% de 65535 -> 1638
    sleep_ms(5000);                 // Aguarda 5 segundos
}


void mover_servo_suavemente_decrescente(uint pino) {

    for (uint i = cento_oitenta_graus; i >= zero_graus; i -= 20) {
        pwm_set_gpio_level(pino, i);
        sleep_ms(10);
    }

}


void mover_servo_suavemente_crescente(uint pino) {

    for (uint i = zero_graus; i <= cento_oitenta_graus; i += 20) {
        pwm_set_gpio_level(pino, i);
        sleep_ms(10);
    }

}