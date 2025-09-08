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
void draw_normal_expression_tick();
void draw_fear_expression_tick();
void limpar_tela();
void draw_laugh_expression_tick();
void draw_eyes_follow_joystick(uint16_t joystick_x, uint16_t joystick_y);
void draw_sleep_expression_tick();
void draw_boredom_arc_eyes();
void st7789_draw_line(int x0, int y0, int x1, int y1, int thickness, uint16_t color);



#ifdef __cplusplus
}
#endif

#endif