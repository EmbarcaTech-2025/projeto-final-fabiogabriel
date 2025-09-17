/**
 * @file mpu6050.h
 * @brief Funções para controle do sensor MPU6050 (acelerômetro e giroscópio).
 */

#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"



void init_i2c(i2c_inst_t *i2c_port, uint sda_pin, uint scl_pin, uint32_t baudrate);
void mpu6050_reset(i2c_inst_t *i2c_port);
void mpu6050_read_raw(i2c_inst_t *i2c_port, int16_t accel[3], int16_t gyro[3], int16_t *temp);



#endif
