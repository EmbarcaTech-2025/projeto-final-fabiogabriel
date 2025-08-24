#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h"
#include "behaviortree.h"
#include "distancia/sensor_distancia.h" // Inclua a biblioteca do sensor

// =========================================================================
//                             FUNÇÃO PRINCIPAL
// =========================================================================

// Definições I2C
#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

// Declaração do sensor como uma variável global para que possa ser acessada por todos os arquivos
VL53L0X sensor;

int main() {
    stdio_init_all();
    
    // Inicialização do Wi-Fi
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    
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

    // Constrói a árvore de comportamento
    Node* tree = build_behavior_tree();

    while(true) {
        printf("\n--- Novo Tick da Arvore ---\n");
        tree->tick_function(tree);
        sleep_ms(100);
    }
    return 0;
}