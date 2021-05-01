#include "draw.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <linux/fb.h>
#include <ft2build.h>
#include <unistd.h>
#include <fcntl.h>
#include FT_FREETYPE_H

#include "layout.h"

static FT_Face face = NULL;

static size_t line_length;

int init_fb(int *x_res, int *y_res, size_t *llength)
{
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	int fbfd = open("/dev/fb0", O_RDWR);
	if (fbfd == -1)
		perror("error: opening framebackbufferfer device /dev/fb0");
	
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("error: reading fixed framebackbufferfer information");
		return -1;
	}
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("error: reading variable framebackbufferfer information");
		return -1;
	}
	line_length = finfo.line_length;
	
	if (llength)
		*llength = line_length;
	
	if (x_res)
		*x_res = vinfo.xres;
	if (y_res)
		*y_res = vinfo.yres;
	return fbfd;
}


bool init_font(const char *font, unsigned int hight)
{
	static FT_Library library;
	if (FT_Init_FreeType(&library)) {
		perror("error: freetype initialization");
		return false;
	}
	if (FT_New_Face(library, font, 0, &face)) {
		perror("unable to load font file");
		return false;
	}
	if (FT_Set_Pixel_Sizes(face, hight/3, hight/3)) {
		perror("FT_Set_Pixel_Sizes failed");
		return false;
	}
	return true;
}


void fill_rect(char *buf, unsigned int x, unsigned int y, unsigned int xend, unsigned int yend, unsigned int color) {
	int i,j;
	int32_t* line;
	for (i=y; i<yend; i++) {
		line = (int32_t *) (buf + line_length*(i));
		for (j=x; j<xend; j++) {
		*(line + j) = color;
		}
	}
}

void draw_char(char *buf, unsigned int x, unsigned int y, unsigned int hight, char c) {
	unsigned int i, j;
	FT_Load_Char(face, c, FT_LOAD_RENDER);
	y += hight*3/4-face->glyph->bitmap_top;
	for (i=0; i<face->glyph->bitmap.rows; i++)
		for (j=0; j<face->glyph->bitmap.width; j++) {
			*(buf + line_length*(i+y) + (j+x)*4) = *(face->glyph->bitmap.buffer + face->glyph->bitmap.pitch*i + j);
			*(buf + line_length*(i+y) + (j+x)*4+1) = *(face->glyph->bitmap.buffer + face->glyph->bitmap.pitch*i + j);
			*(buf + line_length*(i+y) + (j+x)*4+2) = *(face->glyph->bitmap.buffer + face->glyph->bitmap.pitch*i + j);
		}
}

void draw_text(char *buf, unsigned int x, unsigned int y, unsigned int hight, const char *text) {
	while (*text) {
		draw_char(buf, x, y, hight, *text);
		text++;
		x += face->glyph->advance.x >> 6;
	}
}

void draw_keyboard(char *buf, unsigned int width, unsigned int height, int pressed_key, size_t layoutuse)
{
	int keyWidth = width/10;
	int keyHight = height/5;
	int textHight = keyHight*3/4;
	int textOffset = keyWidth/3;
	int specialKeyWidth = width/7;

	for (size_t key=0; key<7; key++) {
		fill_rect(buf, key*specialKeyWidth, 0, (key+1)*specialKeyWidth-1, keyHight*1-1, (key+29==pressed_key)?0xffffff:0x000000);
		draw_text(buf, key*specialKeyWidth+textOffset, 0, textHight, special[key]);
	}
	
	for (size_t key=0; key<10; key++) {
		fill_rect(buf, key*keyWidth, keyHight*1, (key+1)*keyWidth-1, keyHight*2-1, (key==pressed_key)?0xffffff:0x000000);
		draw_char(buf, key*keyWidth+2+textOffset, keyHight*1, textHight, layout[layoutuse][key]);
	}
	
	for (size_t key=0; key<9; key++) {
		fill_rect(buf, keyWidth/2+key*keyWidth, keyHight*2, keyWidth/2+(key+1)*keyWidth-1, keyHight*3-1, (key+10==pressed_key)?0xffffff:0x000000);
		draw_char(buf, keyWidth/2+key*keyWidth+textOffset, keyHight*2, textHight, layout[layoutuse][key+10]);
	}
	
	fill_rect(buf, 0, keyHight*3, width*3/20-1, keyHight*4-1, (28==pressed_key)?0xffffff:0x000000);
	draw_text(buf, textOffset, keyHight*3, textHight, "shift");

	for (size_t key=0; key<7; key++) {
		fill_rect(buf, width*3/20+key*keyWidth, keyHight*3, width*3/20+(key+1)*keyWidth-1, keyHight*4-1, (key+19==pressed_key)?0xffffff:0x000000);
		draw_char(buf, width*3/20+key*keyWidth+textOffset, keyHight*3, textHight, layout[layoutuse][key+19]);
	}
	
	fill_rect(buf, width*17/20, keyHight*3, width-1, keyHight*4-1, (38==pressed_key)?0xffffff:0x000000);
	draw_text(buf, width*17/20+2+textOffset, keyHight*3, textHight, "bksp");
	fill_rect(buf, 0, keyHight*4, width*3/20-1, keyHight*5-1, (39==pressed_key)?0xffffff:0x000000);
	draw_text(buf, textOffset, keyHight*4, textHight, "?123");
	fill_rect(buf, width*3/20, keyHight*4, width*5/20-1, keyHight*5-1, (26==pressed_key)?0xffffff:0x000000);
	draw_char(buf, width*3/20+2+textOffset, keyHight*4, textHight, layout[layoutuse][26]);
	fill_rect(buf, width/4, keyHight*4, width*3/4-1, keyHight*5-1, (36==pressed_key)?0xffffff:0x000000);
	fill_rect(buf, width*3/4, keyHight*4, width*17/20-1, keyHight*5-1, (27==pressed_key)?0xffffff:0x000000);
	draw_char(buf, width*3/4+2+textOffset, keyHight*4, textHight, layout[layoutuse][27]);
	fill_rect(buf, width*17/20, keyHight*4, width-1, keyHight*5-1, (37==pressed_key)?0xffffff:0x000000);
	draw_text(buf, width*17/20+2+textOffset, keyHight*4, textHight, "enter");
}
