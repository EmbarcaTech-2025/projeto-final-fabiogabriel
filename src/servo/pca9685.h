#ifndef PCA9685_H
#define PCA9685_H
#include "hardware/i2c.h"

// Define o endereço da placa
#define PCA9685_ADDR 0x40

#ifdef __cplusplus
extern "C" {
#endif

void pca9685_init(i2c_inst_t *i2c, uint8_t addr);
// Adicione esta linha para definir o pulso em microsegundos
void pca9685_set_servo_angle(i2c_inst_t *i2c, uint8_t addr, uint8_t channel, float angle);
void pca9685_set_pwm(i2c_inst_t *i2c, uint8_t addr, uint8_t channel, uint16_t on, uint16_t off);
#ifdef __cplusplus
}
#endif
#endif