#include "pca9685.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <math.h>

#define MODE1 0x00
#define PRESCALE 0xFE

void pca9685_init(i2c_inst_t *i2c, uint8_t addr) {
    uint8_t buf[2];

    // Resetar e entrar em modo de "sleep"
    buf[0] = MODE1; buf[1] = 0x10;
    i2c_write_blocking(i2c, addr, buf, 2, false);
    sleep_ms(1);

    // Configura a frequência de PWM para 50 Hz, padrão para servos
    buf[0] = PRESCALE; buf[1] = 121;
    i2c_write_blocking(i2c, addr, buf, 2, false);

    // Ativa o PCA9685 e habilita o auto-incremento
    buf[0] = MODE1; buf[1] = 0xA1;
    i2c_write_blocking(i2c, addr, buf, 2, false);
    sleep_ms(1);
}

void pca9685_set_pwm(i2c_inst_t *i2c, uint8_t addr, uint8_t channel, uint16_t on, uint16_t off) {
    uint8_t buf[5];
    buf[0] = 0x06 + 4 * channel; // Registrador do canal
    buf[1] = on & 0xFF;          // ON_L
    buf[2] = on >> 8;            // ON_H
    buf[3] = off & 0xFF;         // OFF_L
    buf[4] = off >> 8;           // OFF_H
    i2c_write_blocking(i2c, addr, buf, 5, false);
}

void pca9685_set_servo_angle(i2c_inst_t *i2c, uint8_t addr, uint8_t channel, float angle) {
    // Garante que o ângulo esteja entre 0 e 180
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    
    // Converte o ângulo para o valor PWM para o SG90
    // Os valores 102 (para 0 graus) e 512 (para 180 graus) são calibrações para 50Hz
    const float pulse_min = 102.0f; 
    const float pulse_max = 512.0f;
    
    uint16_t off = (uint16_t)(pulse_min + (angle / 180.0f) * (pulse_max - pulse_min));
    
    pca9685_set_pwm(i2c, addr, channel, 0, off);
}