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
#include <stdint.h>
#include <linux/input.h>

#define  ROWS 3
#define  COLS 29
#define  SPECIALS 7

extern const char *layout[];

extern  const char *special[SPECIALS];

extern const uint16_t keys[ROWS][COLS];
