#ifndef GPIO_UTILS_H
#define GPIO_UTILS_H

// Define os pinos dos botões e LEDs
#define BUTTON_A       5
#define BUTTON_B       6

#ifdef __cplusplus
extern "C" {
#endif

// Prototipação da função
void init_gpio(void);


#ifdef __cplusplus
}
#endif

#endif // GPIO_UTILS_H