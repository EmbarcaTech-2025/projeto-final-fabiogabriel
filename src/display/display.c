#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdlib.h>
#include <math.h>
#include "display.h"

// === Definições de tela ===
#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240
#define BLACK   0x0000
#define WHITE   0xFFFF

// === Pin Definitions for Raspberry Pi Pico W ===
#define PIN_DIN    19  // MOSI
#define PIN_CLK    18  // SCK
#define PIN_CS     17  // Chip Select
#define PIN_DC     4   // Data/Command
#define PIN_RESET  20  // Reset
#define PIN_BL     9   // Backlight

#define SPI_PORT spi0

// =========================================================================
//                  FUNÇÕES PRIVADAS DO ST7789
// =========================================================================

void st7789_cmd(uint8_t cmd) {
    gpio_put(PIN_DC, 0);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, &cmd, 1);
    gpio_put(PIN_CS, 1);
}

void st7789_data(const uint8_t *data, size_t len) {
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, data, len);
    gpio_put(PIN_CS, 1);
}

void st7789_reset() {
    gpio_put(PIN_RESET, 0);
    sleep_ms(50);
    gpio_put(PIN_RESET, 1);
    sleep_ms(50);
}

void st7789_fill(uint16_t color, int width, int height) {
    st7789_cmd(0x2A); // coluna
    uint8_t data_col[] = {0x00, 0x00, (uint8_t)((width - 1) >> 8), (uint8_t)((width - 1) & 0xFF)};
    st7789_data(data_col, 4);

    st7789_cmd(0x2B); // linha
    uint8_t data_row[] = {0x00, 0x00, (uint8_t)((height - 1) >> 8), (uint8_t)((height - 1) & 0xFF)};
    st7789_data(data_row, 4);

    st7789_cmd(0x2C); // RAM write

    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    for (int i = 0; i < width * height; i++) {
        uint8_t buf[2] = {color >> 8, color & 0xFF};
        spi_write_blocking(SPI_PORT, buf, 2);
    }
    gpio_put(PIN_CS, 1);
}

void st7789_draw_pixel(int x, int y, uint16_t color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

    st7789_cmd(0x2A);
    uint8_t data_col[] = {x >> 8, x & 0xFF, x >> 8, x & 0xFF};
    st7789_data(data_col, 4);

    st7789_cmd(0x2B);
    uint8_t data_row[] = {y >> 8, y & 0xFF, y >> 8, y & 0xFF};
    st7789_data(data_row, 4);

    st7789_cmd(0x2C);
    uint8_t buf[2] = {color >> 8, color & 0xFF};
    gpio_put(PIN_DC, 1);
    gpio_put(PIN_CS, 0);
    spi_write_blocking(SPI_PORT, buf, 2);
    gpio_put(PIN_CS, 1);
}

// Desenha um círculo preenchido (para simular pixels mais grossos)
void st7789_draw_circle_pixel(int cx, int cy, int r, uint16_t color) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                st7789_draw_pixel(cx + x, cy + y, color);
            }
        }
    }
}

// === Funções para desenhar as formas dos olhos ===
void st7789_draw_line(int x0, int y0, int x1, int y1, int thickness, uint16_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        st7789_draw_circle_pixel(x0, y0, thickness/2, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void st7789_draw_right_chevron(int cx, int cy, int size, int slant, int thickness, uint16_t color) {
    st7789_draw_line(cx, cy, cx + size, cy + slant, thickness, color);
    st7789_draw_line(cx, cy, cx + size, cy - slant, thickness, color);
}

void st7789_draw_left_chevron(int cx, int cy, int size, int slant, int thickness, uint16_t color) {
    st7789_draw_line(cx, cy, cx - size, cy + slant, thickness, color);
    st7789_draw_line(cx, cy, cx - size, cy - slant, thickness, color);
}

// Desenha um arco de círculo com espessura (para a boca ou os olhos)
void st7789_draw_arc(int cx, int cy, int r, int start_angle, int end_angle, int thickness, uint16_t color) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            float dist = sqrt(x*x + y*y);
            if (dist > r - thickness && dist < r + thickness) {
                float angle = atan2(y, x) * 180.0 / M_PI;
                if (angle < 0) angle += 360;
                if (angle >= start_angle && angle <= end_angle) {
                    st7789_draw_pixel(cx + x, cy + y, color);
                }
            }
        }
    }
}


// Funções para desenhar os olhos de cada expressão
void draw_circle_eyes(int cx1, int cy1, int cx2, int cy2, int radius, uint16_t color) {
    st7789_draw_circle_pixel(cx1, cy1, radius, color);
    st7789_draw_circle_pixel(cx2, cy2, radius, color);
}

void draw_chevron_eyes(int cx1, int cy1, int cx2, int cy2, int size, int slant, int thickness, uint16_t color) {
    st7789_draw_left_chevron(cx1, cy1, size, slant, thickness, color);
    st7789_draw_right_chevron(cx2, cy2, size, slant, thickness, color);
}

// Desenha a forma de olhos de risada (arcos curvados)
void draw_laugh_eyes(int cx1, int cy1, int cx2, int cy2, int size, int thickness, uint16_t color) {
    // Olho esquerdo
    st7789_draw_arc(cx1, cy1 - size/2, size, 180, 360, thickness, color);
    // Olho direito
    st7789_draw_arc(cx2, cy2 - size/2, size, 180, 360, thickness, color);
}

// === Funções para desenhar os olhos de cada expressão ===
void draw_dizzy_eyes(int cx1, int cy1, int cx2, int cy2, int size, int thickness, uint16_t color) {
    // Olho esquerdo: Desenha dois chevrons para formar um 'X'
    st7789_draw_left_chevron(cx1, cy1, size, size, thickness, color);
    st7789_draw_right_chevron(cx1, cy1, size, size, thickness, color);
    
    // Olho direito: Desenha dois chevrons para formar um 'X'
    st7789_draw_left_chevron(cx2, cy2, size, size, thickness, color);
    st7789_draw_right_chevron(cx2, cy2, size, size, thickness, color);
}

// =========================================================================
//                  FUNÇÕES PÚBLICAS (usadas por main.c e nodes.c)
// =========================================================================

void display_init() {
    spi_init(SPI_PORT, 40000 * 1000);
    gpio_set_function(PIN_CLK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_DIN, GPIO_FUNC_SPI);

    gpio_init(PIN_CS); gpio_set_dir(PIN_CS, GPIO_OUT); gpio_put(PIN_CS, 1);
    gpio_init(PIN_DC); gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_init(PIN_RESET); gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_init(PIN_BL); gpio_set_dir(PIN_BL, GPIO_OUT); gpio_put(PIN_BL, 1);

    st7789_reset();
    st7789_cmd(0x01); // Software reset
    sleep_ms(150);
    st7789_cmd(0x11); // Sleep out
    sleep_ms(150);

    st7789_cmd(0x36); // MADCTL
    uint8_t madctl = 0xA0; // Paisagem 320x240
    st7789_data(&madctl, 1);

    st7789_cmd(0x3A); // Colormode
    uint8_t color = 0x55; // RGB565
    st7789_data(&color, 1);

    st7789_cmd(0x29); // Display ON
    sleep_ms(100);

    st7789_fill(BLACK, SCREEN_WIDTH, SCREEN_HEIGHT); // Fundo preto
}

void limpar_tela(){
    st7789_fill(BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void draw_normal_expression_tick() {
    // Posições dos olhos
    static int eye_x1 = 100;
    static int eye_x2 = 220;
    static const int eye_y = 120;
    static const int eye_radius = 50;
    static int dir = 6;

    // Limpa a tela
    st7789_fill(BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Atualiza posição
    eye_x1 += dir;
    eye_x2 += dir;
    if (eye_x1 < 70 || eye_x2 > 250) {
        dir = -dir;
    }

    // Desenha olhos novos com cor branca
    draw_circle_eyes(eye_x1, eye_y, eye_x2, eye_y, eye_radius, WHITE);

    // Pequeno atraso para a animação ser visível
    sleep_ms(50);
}

void draw_fear_expression_tick() {
    // Variáveis estáticas para manter o estado entre as chamadas da função
    static int eye_x1 = 140;
    static int eye_x2 = 180;
    static int eye_y = 120;
    static int thickness = 7;
    static int eye_size = 80;
    static int eye_slant = 15;

    // Limpa a tela para evitar rastros dos olhos
    st7789_fill(BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Gera um pequeno deslocamento aleatório para o tremor
    int offset_x = (rand() % 11) - 5; // Valores entre -5 e 5
    int offset_y = (rand() % 11) - 5; // Valores entre -5 e 5

    // Desenha os olhos com o deslocamento para criar o efeito trêmulo
    draw_chevron_eyes(eye_x1 + offset_x, eye_y + offset_y, eye_x2 + offset_x, eye_y + offset_y, eye_size, eye_slant, thickness, WHITE);
    
    // Pequeno atraso para a animação ser visível
    sleep_ms(50);
}

void draw_laugh_expression_tick() {
    static int eye_x1 = 100;
    static int eye_x2 = 220;
    static int eye_y = 160; // Posição vertical para a risada
    static int thickness = 7;
    static int eye_size = 40;

    // Limpa a tela
    st7789_fill(BLACK, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Gera um pequeno deslocamento aleatório para o tremor
    int offset_x = (rand() % 11) - 5; // Valores entre -5 e 5
    int offset_y = (rand() % 11) - 5; // Valores entre -5 e 5

    draw_laugh_eyes(eye_x1 + offset_x, eye_y + offset_y, eye_x2 + offset_x, eye_y + offset_y, eye_size, thickness, WHITE);
    
    // Pequeno atraso para a animação ser visível
    sleep_ms(50);
}

void draw_sleep_expression_tick() {
    const int eye_x1 = 100;
    const int eye_x2 = 220;
    const int eye_y = 120;
    const int thickness = 7;
    const int max_size = 40;

    limpar_tela();

    st7789_draw_line(eye_x1 - max_size, eye_y, eye_x1 + max_size, eye_y, thickness, WHITE);
    st7789_draw_line(eye_x2 - max_size, eye_y, eye_x2 + max_size, eye_y, thickness, WHITE);

    sleep_ms(50);
}

void draw_dizzy_expression_tick() {
    static int eye_x1 = 100;
    static int eye_x2 = 220;
    static const int eye_y = 120;
    static const int thickness = 7;
    static const int eye_size = 40;

    limpar_tela();
    draw_dizzy_eyes(eye_x1, eye_y, eye_x2, eye_y, eye_size, thickness, WHITE);
    sleep_ms(50);
}

// Supondo que você tenha a tela inicializada e funções como draw_circle
void draw_eyes_follow_joystick(uint16_t joystick_x, uint16_t joystick_y) {
    printf("Desenhando olhos seguindo o joystick...\n");
    
    limpar_tela();

    // Mapeia a leitura do joystick (0-4095) para a posição dos olhos
    // Supondo que o centro do joystick seja 2047 e a margem de movimento seja 10 pixels
    const int joystick_center = 2047;
    const int eye_max_movement = 30;
    
    int eye_x_offset = -(int)((float)(joystick_y - joystick_center) / (float)joystick_center * eye_max_movement); // inverte esquerda/direita
    int eye_y_offset =  (int)((float)(joystick_x - joystick_center) / (float)joystick_center * eye_max_movement); // mantém cima/baixo


    // Posição base dos olhos (valores de exemplo)
    int left_eye_base_x = 100;
    int left_eye_base_y = 120;
    int right_eye_base_x = 220;
    int right_eye_base_y = 120;

    // Calcula as posições finais
    int left_eye_x = left_eye_base_x + eye_x_offset;
    int left_eye_y = left_eye_base_y + eye_y_offset;
    int right_eye_x = right_eye_base_x + eye_x_offset;
    int right_eye_y = right_eye_base_y + eye_y_offset;

    // Chama sua função para desenhar os olhos
    draw_circle_eyes(left_eye_x, left_eye_y, right_eye_x, right_eye_y, 50, 0xFFFF); // 0xFFFF é branco
    
    sleep_ms(50);
}

void draw_boredom_arc_eyes() {
    const int eye_x1 = 100;
    const int eye_x2 = 220;
    const int eye_y = 120;
    const int thickness = 30;
    const int max_size = 40;

    limpar_tela();

    st7789_draw_line(eye_x1 - max_size, eye_y, eye_x1 + max_size, eye_y, thickness, WHITE);
    st7789_draw_line(eye_x2 - max_size, eye_y, eye_x2 + max_size, eye_y, thickness, WHITE);

    sleep_ms(50);
}

void draw_sapeca_expression() {
    static const int eye_x1 = 100;
    static const int eye_x2 = 220;
    static const int eye_y = 120;
    static const int eye_radius = 50;
    static const int thickness = 7;
    static const int max_size = 40;

    // Variável estática para alternar entre os olhos
    static bool left_eye_open = true;

    limpar_tela();

    if (left_eye_open) {
        // Olho esquerdo aberto (círculo) e direito fechado (linha)
        st7789_draw_circle_pixel(eye_x1, eye_y, eye_radius, WHITE);
        st7789_draw_line(eye_x2 - max_size, eye_y, eye_x2 + max_size, eye_y, thickness, WHITE);
    } else {
        // Olho esquerdo fechado (linha) e direito aberto (círculo)
        st7789_draw_line(eye_x1 - max_size, eye_y, eye_x1 + max_size, eye_y, thickness, WHITE);
        st7789_draw_circle_pixel(eye_x2, eye_y, eye_radius, WHITE);
    }
    
    // Inverte o estado para a próxima chamada
    left_eye_open = !left_eye_open;
    
    sleep_ms(50);
}

void draw_angry_expression_tick() {
    const int eye_x1 = 100;
    const int eye_x2 = 220;
    const int eye_y = 120;
    const int eye_size = 30; // Comprimento da linha do olho
    const int eyebrow_y = 80;
    const int eyebrow_size = 40;
    const int eyebrow_slant = 20; // Inclinação da sobrancelha
    const int thickness = 7;
    
    limpar_tela();

    // Desenha os olhos (linhas horizontais finas)
    st7789_draw_line(eye_x1 - eye_size, eye_y, eye_x1 + eye_size, eye_y, thickness, WHITE);
    st7789_draw_line(eye_x2 - eye_size, eye_y, eye_x2 + eye_size, eye_y, thickness, WHITE);

    // Desenha as sobrancelhas (linhas inclinadas para fora)
    st7789_draw_line(eye_x1 - eyebrow_size, eyebrow_y, eye_x1 + eyebrow_size, eyebrow_y + eyebrow_slant, thickness, WHITE);
    st7789_draw_line(eye_x2 - eyebrow_size, eyebrow_y + eyebrow_slant, eye_x2 + eyebrow_size, eyebrow_y, thickness, WHITE);

    sleep_ms(50);
}


void draw_look_around_expression_tick() {
    static const int eye_base_x1 = 100;
    static const int eye_base_x2 = 220;
    static const int eye_base_y = 120;
    static const int offset = 20;

    // Array de 8 posições (offsets X, Y)
    static const int positions[8][2] = {
        {0, -offset},   // cima
        {0, offset},    // baixo
        {-offset, 0},   // esquerda
        {offset, 0},    // direita
        {-offset, -offset}, // cima_esq
        {offset, -offset},  // cima_dir
        {offset, offset},   // baixo_dir
        {-offset, offset}   // baixo_esq
    };

    static int current_pos_index = 0;
    static int counter = 0;
    const int change_threshold = 10; // Mudar de posição a cada 10 "ticks"

    limpar_tela();

    // Checa se é hora de mudar para uma nova posição aleatória
    if (counter >= change_threshold) {
        current_pos_index = rand() % 8;
        counter = 0;
    } else {
        counter++;
    }

    // Calcula a nova posição dos olhos com base no array
    int eye_x1 = eye_base_x1 + positions[current_pos_index][0];
    int eye_x2 = eye_base_x2 + positions[current_pos_index][0];
    int eye_y = eye_base_y + positions[current_pos_index][1];

    draw_circle_eyes(eye_x1, eye_y, eye_x2, eye_y, 50, WHITE);
    sleep_ms(50);
}
