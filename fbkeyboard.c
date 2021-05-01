/*
fbkeyboard.c : framebackbufferfer softkeyboard for touchscreen devices

Copyright 2017 Julian Winkler <julia.winkler1@web.de>

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

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "layout.h"
#include "draw.h"
#include "uinput.h"

const char *font = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
const char *device = "/dev/input/by-path/platform-mapphone_touchscreen-event";


int main(int argc, char *argv[])
{
	int layoutuse = 0;

	size_t xres, yres, line_length;
	int fbfd = init_fb(&xres, &yres, &line_length);
	int hight = (yres / 3 + xres / 2) / 2;

	if (!init_font(font, hight/5))
		return -1;
	
	if (!init_uinput())
		return -2;

	int fdinput;
	struct input_absinfo abs_x, abs_y;
	int absolute_x, absolute_y;
	int touchdown, pressed = -1, released, key;
	struct input_event ie;
	
	if (device) {
		if ((fdinput = open(device, O_RDONLY)) == -1) {
			perror("failed to open input device node");
			return -1;
		}
	} else {
		DIR *inputdevs = opendir("/dev/input");
		struct dirent *dptr;
		fdinput = -1;
		while (dptr = readdir(inputdevs)) {
			if ((fdinput = openat(dirfd(inputdevs), dptr->d_name, O_RDONLY)) != -1
			    && ioctl(fdinput, EVIOCGBIT(0, sizeof(key)), &key) != -1 && key >> EV_ABS & 1)
				break;
			if (fdinput != -1) {
				close(fdinput);
				fdinput = -1;
			}
		}
		if (fdinput == -1) {
			fprintf(stderr, "no absolute axes device found in /dev/input\n");
			return -1;
		}
	}
	if (ioctl(fdinput, EVIOCGABS(ABS_MT_POSITION_X), &abs_x) == -1)
		perror("error: getting touchscreen size");
	if (ioctl(fdinput, EVIOCGABS(ABS_MT_POSITION_Y), &abs_y) == -1)
		perror("error: getting touchscreen size");

	char *backbuffer = malloc(line_length * hight);
	if (backbuffer == 0) {
		perror("OOM!");
		return -1;
	}
	//fill background gray
	fill_rect(backbuffer, 0, 0, xres, hight, 0x444444);

	while (true) {

		draw_keyboard(backbuffer, xres, hight, pressed, layoutuse);
		lseek(fbfd, line_length * (yres - hight), SEEK_SET);
		write(fbfd, backbuffer, line_length * hight);

		released = 0;
		while (read(fdinput, &ie, sizeof(struct input_event)) && !(ie.type == EV_SYN && ie.code == SYN_REPORT)) {
			if (ie.type == EV_ABS) {
				switch (ie.code) {
				case ABS_MT_POSITION_X:
					absolute_x = ie.value;
					touchdown = 1;
					key = 0;
					break;
				case ABS_MT_POSITION_Y:
					absolute_y = ie.value;
					touchdown = 1;
					key = 0;
					break;
				case ABS_MT_TRACKING_ID:
					if (ie.value == -1) {
						touchdown = 0;
						released = 1;
					}
					break;
				}
			}
			if (ie.type == EV_SYN && ie.code == SYN_MT_REPORT && key) {
				touchdown = 0;
				released = 1;
			}
		}

		if (released && pressed != -1) {
			if (pressed <= 27)	//normal keys (abc...)
				uinput_send_key(keys[layoutuse >> 1][pressed]);
			else if (pressed == 28) {
				layoutuse ^= 1;
				uinput_send_modifier(KEY_LEFTCTRL,  layoutuse & 1);
			}
			else if (pressed == 29)
				uinput_send_modifier(KEY_LEFTCTRL, 1);
			else if (pressed <= 38)	//special keys (space, tab, ...)
				uinput_send_key(keys[2][pressed - 28]);
			else if (pressed == 39)	//second page
				layoutuse ^= 2;
		}

		pressed = -1;
		if (touchdown) {
			switch (((absolute_y - abs_y.maximum) * yres +
				 abs_y.maximum * hight) / (hight/5) / abs_y.maximum) {
			case 0:
				pressed = 29 + absolute_x * 7 / abs_x.maximum;
				break;
			case 1:
				pressed = absolute_x * 10 / abs_x.maximum;
				break;
			case 2:
				if (absolute_x > abs_x.maximum / 20 && absolute_x < abs_x.maximum * 19 / 20)
					pressed = 10 + (absolute_x * 10 - abs_x.maximum / 2) / abs_x.maximum;
				break;
			case 3:
				if (absolute_x < abs_x.maximum * 3 / 20)
					pressed = 28;	//shift
				else if (absolute_x < abs_x.maximum * 17 / 20)
					pressed = 19 + (absolute_x * 10 - abs_x.maximum * 3 / 2) / abs_x.maximum;
				else
					pressed = 38;	//bksp
				break;
			case 4:
				if (absolute_x < abs_x.maximum * 3 / 20)
					pressed = 39;	//123
				else if (absolute_x < abs_x.maximum * 5 / 20)
					pressed = 26;
				else if (absolute_x < abs_x.maximum * 15 / 20)
					pressed = 36;	//space
				else if (absolute_x < abs_x.maximum * 17 / 20)
					pressed = 27;
				else
					pressed = 37;	//enter
				break;
			}
		}
	}
}
