CC       := gcc
SOURCES    := ${wildcard *.c}
OBJECTS    := $(SOURCES:%.c=%.o)
FT2_CFLAGS := $(shell pkg-config --cflags freetype2)
FT2_LIBS  := $(shell pkg-config --libs freetype2)
INPIUT_CFLAGS := $(shell pkg-config --cflags libinput)
INPIUT_LIBS  := $(shell pkg-config --libs libinput)
UDEV_LIBS  := $(shell pkg-config --libs libudev)
UDEV_CFLAGS  := $(shell pkg-config --cflags libudev)
LIBS	:= $(FT2_LIBS) $(INPIUT_LIBS) $(UDEV_LIBS)
CCFLAGS   := -g -I. $(FT2_CFLAGS) $(INPIUT_CFLAGS) $(UDEV_CFLAGS)

all: fbkeyboard

%.o: %.c
	@echo CC $<
	@$(CC) -c -o $@ $< $(CCFLAGS)

fbkeyboard: $(OBJECTS)
	@echo LD $@
	@$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

clean:
	-rm -fv *.o fbkeyboard
