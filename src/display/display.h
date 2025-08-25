#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <stddef.h>

// Definições de cores RGB565
#define BLACK   0x0000
#define WHITE   0xFFFF

// Use extern "C" para que as funções sejam acessíveis pelo C++
// sem name mangling.
#ifdef __cplusplus
extern "C" {
#endif

// Funções públicas da API do display
void display_init();
void draw_fear_expression_tick();

#ifdef __cplusplus
}
#endif

#endif