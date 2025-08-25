#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "display.h"

// === Definições de tela ===
#define SCREEN_WIDTH   320
#define SCREEN_HEIGHT  240

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

void st7789_draw_circle(int cx, int cy, int r, uint16_t color) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                st7789_draw_pixel(cx + x, cy + y, color);
            }
        }
    }
}

void draw_eyes(int cx1, int cy1, int cx2, int cy2, uint16_t color) {
    int eye_radius = 50;
    st7789_draw_circle(cx1, cy1, eye_radius, color);
    st7789_draw_circle(cx2, cy2, eye_radius, color);
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

void draw_fear_expression_tick() {
    static int eye_x1 = 100;
    static int eye_x2 = 220;
    static int eye_y = 120;
    static int dir = 4;

    // Apaga olhos antigos (desenha com cor preta)
    draw_eyes(eye_x1, eye_y, eye_x2, eye_y, BLACK);

    // Atualiza posição
    eye_x1 += dir;
    eye_x2 += dir;
    if (eye_x1 < 70 || eye_x2 > 250) {
        dir = -dir;
    }

    // Desenha olhos novos com cor branca
    draw_eyes(eye_x1, eye_y, eye_x2, eye_y, WHITE);

    // Pequeno atraso para a animação ser visível
    sleep_ms(50);
}