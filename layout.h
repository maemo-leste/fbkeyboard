#pragma once
#include <stdint.h>
#include <linux/input.h>

#define  ROWS 3
#define  COLS 29
#define  SPECIALS 7

extern const char *layout[];

extern  const char *special[SPECIALS];

extern const uint16_t keys[ROWS][COLS];
