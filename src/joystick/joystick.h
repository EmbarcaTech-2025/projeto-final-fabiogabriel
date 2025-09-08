#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

// Pinos do joystick
#define JOYSTICK_VRX_PIN 26
#define JOYSTICK_VRY_PIN 27
#define JOYSTICK_SW_PIN 22

// Estrutura para armazenar o estado do joystick
typedef struct {
    uint16_t x_value;
    uint16_t y_value;
    bool button_pressed;
} joystick_state_t;

// Funções para manipulação do joystick
void init_joystick();
joystick_state_t read_joystick_state();

#ifdef __cplusplus
}
#endif

#endif
