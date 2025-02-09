#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

const uint pino_servo = 22;
const uint pino_led = 11;


void setup_led();


int main()
{
    stdio_init_all();

    setup_led();

    while (true) {
        sleep_ms(1000);
    }
}


/*
* Função para configurar o pino do led
*/
void setup_led() {
    gpio_init(pino_led);
    gpio_set_dir(pino_led, GPIO_OUT);
    gpio_put(pino_led, 0);
}