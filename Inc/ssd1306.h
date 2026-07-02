
#ifndef __SSD1306_H
#define __SSD1306_H

#include "main.h"

#define SSD1306_I2C_ADDR (0x3C << 1)
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

typedef enum
{
    Black = 0,
    White = 1
} SSD1306_COLOR;

void SSD1306_Init(void);
void SSD1306_Clear(void);
void SSD1306_UpdateScreen(void);
void SSD1306_GotoXY(uint8_t x, uint8_t y);
void SSD1306_Puts(char *str, SSD1306_COLOR color);

#endif
