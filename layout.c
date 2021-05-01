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

#include "layout.h"

const char *layout[] = { "qwertyuiopasdfghjklzxcvbnm/.",
	"QWERTYUIOPASDFGHJKLZXCVBNM?>",
	"1234567890-={};\'\\,./      /.",
	"!@#$%^&*()_+[]:\"|<>?      ?>"
};

const char *special[SPECIALS] = { "ctl", "esc", "tab", "up", "down", "left", "right" };

const uint16_t keys[ROWS][COLS] =
    { {KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H,
       KEY_J, KEY_K, KEY_L, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_SLASH, KEY_DOT},
{KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL, KEY_LEFTBRACE,
 KEY_RIGHTBRACE, KEY_SEMICOLON, KEY_APOSTROPHE, KEY_BACKSLASH, KEY_COMMA, KEY_DOT, KEY_SLASH, KEY_X, KEY_C, KEY_V,
 KEY_B, KEY_N, KEY_M, KEY_SLASH, KEY_DOT},
{KEY_LEFTSHIFT, KEY_LEFTCTRL, KEY_ESC, KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE, KEY_ENTER,
 KEY_BACKSPACE}
};
