/*
This file is part of fbkeyboard

Copyright 2017 Julian Winkler <julia.winkler1@web.de>
Copyright 2021 Carl Philipp Klemm <carl@uvos.xyz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <stddef.h>
#include <stdbool.h>

bool init_font(const char *font, unsigned int hight);

void fill_rect(char *buf, unsigned int x, unsigned int y, unsigned int xend, unsigned int yend, unsigned int color);

void draw_char(char *buf, unsigned int x, unsigned int y, unsigned int hight, char c);

void draw_text(char *buf, unsigned int x, unsigned int y, unsigned int hight, const char *text);

void draw_keyboard(char *buf, unsigned int width, unsigned int height, int pressed_key, size_t layoutuse);

int init_fb(int *x_res, int *y_res, size_t *llength);
