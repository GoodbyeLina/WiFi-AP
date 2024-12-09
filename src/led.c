#include "pico/stdlib.h"
#include <stdlib.h>
#include <stdio.h>

void led_init(){
    
    gpio_init(PIN_LED_PICO);
    gpio_set_dir(PIN_LED_PICO, GPIO_OUT);
    // gpio_put(PIN_LED_PICO, true);

}

void led_blink_ms(int time){

    gpio_put(PIN_LED_PICO, true);
    sleep_ms(time);
    gpio_put(PIN_LED_PICO, false);
    sleep_ms(time);

}