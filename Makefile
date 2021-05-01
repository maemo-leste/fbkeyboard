CC       := gcc
SOURCES    := ${wildcard *.c}
OBJECTS    := $(SOURCES:%.c=%.o)
FT2_CFLAGS := $(shell pkg-config --cflags freetype2)
FT2_LIBS  := $(shell pkg-config --libs freetype2)
INPIUT_CFLAGS := $(shell pkg-config --cflags libinput)
INPIUT_LIBS  := $(shell pkg-config --libs libinput)
LIBS	:= $(FT2_LIBS)
CCFLAGS   := -g -I. $(FT2_CFLAGS) $(INPIUT_CFLAGS)

all: fbkeyboard

%.o: %.c
	@echo CC $<
	@$(CC) -c -o $@ $< $(CCFLAGS)

fbkeyboard: $(OBJECTS)
	@echo LD $@
	@$(CC) -o $@ $^ $(CCFLAGS) $(LIBS)

clean:
	-rm -fv *.o fbkeyboard
