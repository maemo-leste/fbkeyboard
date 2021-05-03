/*
fbkeyboard.c : framebackbufferfer softkeyboard for touchscreen devices

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
#include <libinput.h>
#include <errno.h>
#include <libudev.h>
#include <sys/poll.h>

#include "layout.h"
#include "draw.h"
#include "uinput.h"


const char *font = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";

static int open_restricted(const char *path, int flags, void *user_data)
{
	int fd = open(path, flags);
	return fd < 0 ? -errno : fd;
}
 
static void close_restricted(int fd, void *user_data)
{
	close(fd);
}
 
const static struct libinput_interface interface = {
	.open_restricted = open_restricted,
	.close_restricted = close_restricted,
};

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("usage: %s [FILE]\nA font file is required.\n", argv[0]);
		return -1;
	}
	
	int layoutuse = 0;

	size_t xres, yres, line_length;
	int fbfd = init_fb(&xres, &yres, &line_length);
	int hight = (yres / 3 + xres / 2) / 2;

	if (!init_font(argv[1], hight/5))
		return -1;
	
	if (!init_uinput())
		return -2;

	struct udev *udev_cntxt = udev_new();
	struct libinput *li;
	struct libinput_event *event = NULL;
	if (!udev_cntxt)
		return -3;
	
	if(!(li = libinput_udev_create_context(&interface, NULL, udev_cntxt)))
		return -4;
	
	libinput_udev_assign_seat(li, "seat0");
	int lifd = libinput_get_fd(li);
	if (lifd < 0)
		return -5;

	char *backbuffer = malloc(line_length * hight);
	if (backbuffer == 0) {
		perror("OOM!");
		return -1;
	}
	//fill background gray
	fill_rect(backbuffer, 0, 0, xres, hight, 0x444444);
	
	int pressed = -1;
	
	draw_keyboard(backbuffer, xres, hight, pressed, layoutuse);
	lseek(fbfd, line_length * (yres - hight), SEEK_SET);
	write(fbfd, backbuffer, line_length * hight);

	while (true) {
		
		struct pollfd fds;
		fds.fd = lifd;
		fds.events = POLLIN;
	        fds.revents = 0;
		poll(&fds, 1, -1);
		libinput_dispatch(li);
		while((event = libinput_get_event(li)))
		{
			int absolute_x = 0;
			int absolute_y = 0;
			bool touchdown = false;
			bool released = false;
			if (libinput_event_get_type(event) == LIBINPUT_EVENT_TOUCH_DOWN) {
				touchdown = true;
				struct libinput_event_touch *touch = libinput_event_get_touch_event(event);
				if (touch) {
					absolute_x = libinput_event_touch_get_x_transformed(touch, xres);
					absolute_y = libinput_event_touch_get_y_transformed(touch, yres);
				}
			 }
			 else if(libinput_event_get_type(event) == LIBINPUT_EVENT_TOUCH_UP) {
				 released = true;
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
				
				pressed = -1;
			}

			
			if (touchdown) {
				switch (((absolute_y - yres) * yres +
					 yres * hight) / (hight/5) / yres) {
				case 0:
					pressed = 29 + absolute_x * 7 / xres;
					break;
				case 1:
					pressed = absolute_x * 10 / xres;
					break;
				case 2:
					if (absolute_x > xres / 20 && absolute_x < xres * 19 / 20)
						pressed = 10 + (absolute_x * 10 - xres / 2) / xres;
					break;
				case 3:
					if (absolute_x < xres * 3 / 20)
						pressed = 28;	//shift
					else if (absolute_x < xres * 17 / 20)
						pressed = 19 + (absolute_x * 10 - xres * 3 / 2) / xres;
					else
						pressed = 38;	//bksp
					break;
				case 4:
					if (absolute_x < xres * 3 / 20)
						pressed = 39;	//123
					else if (absolute_x < xres * 5 / 20)
						pressed = 26;
					else if (absolute_x < xres * 15 / 20)
						pressed = 36;	//space
					else if (absolute_x < xres * 17 / 20)
						pressed = 27;
					else
						pressed = 37;	//enter
					break;
				}
			}
		}
		draw_keyboard(backbuffer, xres, hight, pressed, layoutuse);
		lseek(fbfd, line_length * (yres - hight), SEEK_SET);
		write(fbfd, backbuffer, line_length * hight);
	}
	
	libinput_unref(li);
	udev_unref(udev_cntxt);
	
	return 0;
}
