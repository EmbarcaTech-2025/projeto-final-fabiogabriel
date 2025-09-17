#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h"
#include "behavior/behaviortree.h"
#include "distancia/sensor_distancia.h" // Inclua a biblioteca do sensor
#include "display/display.h" 
#include "botoes/gpio.h"
#include "joystick/joystick.h"

#include "oled/ssd1306.h"
#include "giroscópio/mpu6050.h"
#include "buzzer/som.h"
#include "servo/pca9685.h"


// =========================================================================
//                             FUNÇÃO PRINCIPAL
// =========================================================================

// Definições I2C
#define I2C_PORT i2c1
#define I2C_SDA 2
#define I2C_SCL 3
#define BUTTON_A 5
#define BUTTON_B 6
#define BUZZER_PIN 21


// Declaração do sensor como uma variável global para que possa ser acessada por todos os arquivos
VL53L0X sensor(I2C_PORT, VL53L0X_DEFAULT_ADDRESS);

volatile bool medo_ativo = false;



int main() {
    stdio_init_all();
    
    // Inicialização do Wi-Fi
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    
    init_joystick();
    init_gpio();
    // Inicialização do I2C para o sensor VL53L0X
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do sensor VL53L0X
    sensor.init();
    sensor.setTimeout(500);
    sensor.startContinuous(100);

    display_init();

    pwm_init_buzzer(BUZZER_PIN);

    mpu6050_reset(I2C_PORT);

    int16_t acceleration[3], gyro[3], temp;
    
    pca9685_init(I2C_PORT, PCA9685_ADDR);
    // Constrói a árvore de comportamento
    Node* tree = build_behavior_tree();

    while(true) {
        printf("\n--- Novo Tick da Arvore ---\n");
        tree->tick_function(tree);
        pca9685_init(I2C_PORT, PCA9685_ADDR);
        sleep_ms(100);
    }
    return 0;
}