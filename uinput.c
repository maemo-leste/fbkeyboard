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

#include "uinput.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#include "layout.h"

static int fduinput;

bool init_uinput(void)
{
	fduinput = open("/dev/uinput", O_WRONLY);
	if (fduinput == -1) {
		perror("error: cannot open uinput device /dev/uinput");
		return false;
	}
	if (ioctl(fduinput, UI_SET_EVBIT, EV_KEY) == -1) {
		perror("error: SET_EVBIT EV_KEY");
		return false;
	}
	if (ioctl(fduinput, UI_SET_EVBIT, EV_SYN) == -1) {
		perror("error: SET_EVBIT EV_SYN");
		return false;
	}
	for (int key = 0; key < 28; key++)
		ioctl(fduinput, UI_SET_KEYBIT, keys[0][key]);
	for (int key = 0; key < 18; key++)
		ioctl(fduinput, UI_SET_KEYBIT, keys[1][key]);
	for (int key = 0; key < 11; key++)
		ioctl(fduinput, UI_SET_KEYBIT, keys[2][key]);
	struct uinput_user_dev uidev;
	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "fbkeyboard");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 1;
	uidev.id.product = 1;
	uidev.id.version = 1;
	if (write(fduinput, &uidev, sizeof(uidev)) != sizeof(uidev)) {
		fprintf(stderr, "error setting up uinput\n");
		return false;
	}
	if (ioctl(fduinput, UI_DEV_CREATE) == -1) {
		perror("error creating uinput dev");
		return false;
	}
	return true;
}

void uinput_send_modifier(__u16 code, __u16 value)
{
	struct input_event ie = {};
	ie.type = EV_KEY;
	ie.code = code;
	ie.value = value;
	if (write(fduinput, &ie, sizeof(ie)) != sizeof(ie))
		fprintf(stderr, "error sending uinput event\n");
}

void uinput_send_key(__u16 code)
{
	struct input_event ie = {};
	ie.type = EV_KEY;
	ie.code = code;
	ie.value = 1;
	if (write(fduinput, &ie, sizeof(ie)) != sizeof(ie))
		fprintf(stderr, "error: sending uinput event\n");
	ie.value = 0;
	if (write(fduinput, &ie, sizeof(ie)) != sizeof(ie))
		fprintf(stderr, "error: sending uinput event\n");
	ie.code = KEY_LEFTCTRL;
	if (write(fduinput, &ie, sizeof(ie)) != sizeof(ie))
		fprintf(stderr, "error: sending uinput event\n");
	ie.type = EV_SYN;
	ie.code = SYN_REPORT;
	if (write(fduinput, &ie, sizeof(ie)) != sizeof(ie))
		fprintf(stderr, "error: sending uinput event\n");
}
