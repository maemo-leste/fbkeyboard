#pragma once
#include <stdbool.h>
#include <linux/uinput.h>

bool init_uinput(void);

void uinput_send_key(__u16 code);

void uinput_send_modifier(__u16 code, __u16 value);
