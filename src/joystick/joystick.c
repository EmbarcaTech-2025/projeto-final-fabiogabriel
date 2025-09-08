#include "hardware/adc.h"
#include "hardware/gpio.h"

#define JOYSTICK_VRX_PIN 26
#define JOYSTICK_VRY_PIN 27
#define JOYSTICK_SW_PIN 22

typedef struct {
    uint16_t x_value;
    uint16_t y_value;
    bool button_pressed;
} joystick_state_t;

/**
 * @brief Inicializa o hardware do joystick.
 * Configura os pinos VRX e VRY como entradas ADC e o pino SW como entrada digital com pull-up.
 */
void init_joystick() {
    // Inicializa o ADC e os pinos de entrada analógica
    adc_init();
    adc_gpio_init(JOYSTICK_VRX_PIN);
    adc_gpio_init(JOYSTICK_VRY_PIN);

    // Inicializa o pino do botão do joystick
    gpio_init(JOYSTICK_SW_PIN);
    gpio_set_dir(JOYSTICK_SW_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW_PIN);
}

/**
 * @brief Lê o estado atual do joystick (eixos X, Y e botão).
 * @return Uma estrutura joystick_state_t contendo os valores lidos.
 */
joystick_state_t read_joystick_state() {
    joystick_state_t state;

    // Leitura do valor do eixo X do joystick
    adc_select_input(0); // ADC0 corresponde ao pino 26 (VRX)
    state.x_value = adc_read();

    // Leitura do valor do eixo Y do joystick
    adc_select_input(1); // ADC1 corresponde ao pino 27 (VRY)
    state.y_value = adc_read();

    // Leitura do valor do botão do joystick (invertido por causa do pull-up)
    state.button_pressed = !gpio_get(JOYSTICK_SW_PIN);

    return state;
}
