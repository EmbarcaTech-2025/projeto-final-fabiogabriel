/**
 * @file mpu6050.c
 * @brief Implementação das funções de controle para o sensor MPU6050.
 */

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>

#include "mpu6050.h"


// Endereço I2C do MPU6050
#define MPU6050_ADDRESS 0x68

void init_i2c(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin, uint32_t baudrate) {
    i2c_init(i2c_port, baudrate);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
}

void mpu6050_reset(i2c_inst_t *i2c_port) {
    uint8_t buf[] = {0x6B, 0x80}; // Reset
    i2c_write_blocking(i2c_port, MPU6050_ADDRESS, buf, 2, false);
    sleep_ms(100);

    buf[1] = 0x00; // Clear sleep mode
    i2c_write_blocking(i2c_port, MPU6050_ADDRESS, buf, 2, false);
    sleep_ms(10);
}

void mpu6050_read_raw(i2c_inst_t *i2c_port, int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    uint8_t buffer[6];
    uint8_t val;

    // Aceleração
    val = 0x3B;
    i2c_write_blocking(i2c_port, MPU6050_ADDRESS, &val, 1, true);
    i2c_read_blocking(i2c_port, MPU6050_ADDRESS, buffer, 6, false);
    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Giroscópio
    val = 0x43;
    i2c_write_blocking(i2c_port, MPU6050_ADDRESS, &val, 1, true);
    i2c_read_blocking(i2c_port, MPU6050_ADDRESS, buffer, 6, false);
    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
    }

    // Temperatura
    val = 0x41;
    i2c_write_blocking(i2c_port, MPU6050_ADDRESS, &val, 1, true);
    i2c_read_blocking(i2c_port, MPU6050_ADDRESS, buffer, 2, false);
    *temp = buffer[0] << 8 | buffer[1];
}
