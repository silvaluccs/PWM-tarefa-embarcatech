#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

const uint pino_servo = 22;
const uint pino_led = 11;

void setup_led();
void setup_pwm(uint pino, uint *slice);
void girar_servo_180(uint pino, uint slice);
void girar_servo_90(uint pino, uint slice);
void girar_servo_0(uint pino, uint slice);


int main() {
    stdio_init_all();

    setup_led();

    uint slice;
    setup_pwm(pino_servo, &slice);

    while (true) {
        printf("Girando servo 180 graus\n");
        girar_servo_90(pino_servo, slice);
        sleep_ms(5000);  // Aguarda 5 segundos
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
    pwm_set_clkdiv(*slice, 625.f);           // Define o divisor de clock do PWM
    pwm_set_wrap(*slice, 4000);              // Define o valor de wrap (4000 passos)
}

/*
 * Função para girar o servo para 180 graus (Duty Cycle de 12%)
 */
void girar_servo_180(uint pino, uint slice) {
    pwm_set_gpio_level(pino, 480);  // 12% de 4000 -> 480
    pwm_set_enabled(slice, true);   // Habilita o PWM no slice correspondente
    sleep_ms(5000);                 // Aguarda 5 segundos
}

void girar_servo_90(uint pino, uint slice) {
    pwm_set_gpio_level(pino, 294);  // 7,35% de 4000 -> 294
    pwm_set_enabled(slice, true);   // Habilita o PWM no slice correspondente
    sleep_ms(5000);                 // Aguarda 5 segundos
}

/*
 * Função para girar o servo para 0 graus (Duty Cycle de 2,5%)
 */
void girar_servo_0(uint pino, uint slice) {
    pwm_set_gpio_level(pino, 100);  // 2,5% de 4000 -> 100
    pwm_set_enabled(slice, true);   // Habilita o PWM no slice correspondente
    sleep_ms(5000);                 // Aguarda 5 segundos
}
