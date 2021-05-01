#pragma once
#include <stddef.h>
#include <stdbool.h>

bool init_font(const char *font, unsigned int hight);

void fill_rect(char *buf, unsigned int x, unsigned int y, unsigned int xend, unsigned int yend, unsigned int color);

void draw_char(char *buf, unsigned int x, unsigned int y, unsigned int hight, char c);

void draw_text(char *buf, unsigned int x, unsigned int y, unsigned int hight, const char *text);

void draw_keyboard(char *buf, unsigned int width, unsigned int height, int pressed_key, size_t layoutuse);

int init_fb(int *x_res, int *y_res, size_t *llength);
