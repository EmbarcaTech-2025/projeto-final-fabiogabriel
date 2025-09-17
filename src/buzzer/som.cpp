#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "som.h"

// Definições de notas musicais para a música tema de Star Wars
const uint star_wars_notes[] = {
    SOL, RE, RE, LA, RE, SI,
    RE, SI, DO, RE,
    SOL, RE, RE, LA, RE, SI,  // repetição 2x
    SOL, RE, RE, LA, RE, SI,
    
    RE, DO, RE, SI, RE, SOL,
    RE, DO, RE, SI, RE, LA,
    RE, DO, RE, SI, RE, SOL,
    LA, SOL, LA, SOL, SI, SOL
};

// Duração das notas em milissegundos
const uint star_wars_duration[] = {
    250, 250, 250, 250, 250, 250,
    250, 250, 250, 250,
    250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250,
    
    250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250,
    250, 250, 250, 250, 250, 250
};

// Inicializa o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

// Toca uma nota com a frequência e duração especificadas
void play_tone(uint pin, uint frequency, uint duration_ms) {
    if (frequency == 0) {
        pwm_set_gpio_level(pin, 0);
        sleep_ms(duration_ms);
        return;
    }

    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint32_t clock_freq = clock_get_hz(clk_sys);
    uint32_t top = clock_freq / frequency - 1;

    pwm_set_wrap(slice_num, top);
    pwm_set_gpio_level(pin, top / 200);

    sleep_ms(duration_ms);

    pwm_set_gpio_level(pin, 0);
    sleep_ms(20);
}

// Toca a música tema de Star Wars
void play_star_wars_theme(uint pin) {
    for (int i = 0; i < sizeof(star_wars_notes) / sizeof(star_wars_notes[0]); i++) {
        play_tone(pin, star_wars_notes[i], star_wars_duration[i]);
    }

}

void play_risada_sound(uint pin) {
    play_tone(pin, 330, 50);  // Primeiro "ping"
    play_tone(pin, 2000, 50);  // Segundo "ping"
    play_tone(pin, 2200, 50);  // Terceiro "ping"

}
void play_angry_sound(uint pin) {
    play_tone(pin, 230, 50);  // Primeiro "ping"
    play_tone(pin, 230, 50);  // Primeiro "ping"

}
void play_dizzy_sound(uint pin) {
    play_tone(pin, 330, 50);  // Primeiro "ping"
    play_tone(pin, 300, 50);  // Primeiro "ping"

}
void play_medo_sound(uint pin) {
    play_tone(pin, 500, 50);  // Primeiro "ping"
    play_tone(pin, 500, 50);  // Primeiro "ping"
    play_tone(pin, 500, 50);  // Primeiro "ping"

}
void play_piscada_sound(uint pin) {
    play_tone(pin, 8000, 50);  // Primeiro "ping"

}