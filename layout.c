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
