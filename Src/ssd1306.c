#include <ssd1306.h>
#include <string.h>

extern I2C_HandleTypeDef hi2c1;

static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static uint8_t CurrentX;
static uint8_t CurrentY;

static const uint8_t Font5x7[][5] =
{
    {0x00,0x00,0x00,0x00,0x00}, // space
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}, // 9

    {0x7E,0x11,0x11,0x11,0x7E}, // A
    {0x7F,0x49,0x49,0x49,0x36}, // B
    {0x3E,0x41,0x41,0x41,0x22}, // C
    {0x7F,0x41,0x41,0x22,0x1C}, // D
    {0x7F,0x49,0x49,0x49,0x41}, // E
    {0x7F,0x09,0x09,0x09,0x01}, // F
    {0x3E,0x41,0x49,0x49,0x7A}, // G
    {0x7F,0x08,0x08,0x08,0x7F}, // H
    {0x00,0x41,0x7F,0x41,0x00}, // I
    {0x20,0x40,0x41,0x3F,0x01}, // J
    {0x7F,0x08,0x14,0x22,0x41}, // K
    {0x7F,0x40,0x40,0x40,0x40}, // L
    {0x7F,0x02,0x0C,0x02,0x7F}, // M
    {0x7F,0x04,0x08,0x10,0x7F}, // N
    {0x3E,0x41,0x41,0x41,0x3E}, // O
    {0x7F,0x09,0x09,0x09,0x06}, // P
    {0x3E,0x41,0x51,0x21,0x5E}, // Q
    {0x7F,0x09,0x19,0x29,0x46}, // R
    {0x46,0x49,0x49,0x49,0x31}, // S
    {0x01,0x01,0x7F,0x01,0x01}, // T
    {0x3F,0x40,0x40,0x40,0x3F}, // U
    {0x1F,0x20,0x40,0x20,0x1F}, // V
    {0x7F,0x20,0x18,0x20,0x7F}, // W
    {0x63,0x14,0x08,0x14,0x63}, // X
    {0x07,0x08,0x70,0x08,0x07}, // Y
    {0x61,0x51,0x49,0x45,0x43}, // Z

    {0x00,0x36,0x36,0x00,0x00}, // :
    {0x00,0x00,0x5F,0x00,0x00}, // !
    {0x40,0x40,0x40,0x40,0x40}, // -
    {0x60,0x60,0x00,0x00,0x00}, // .
    {0x14,0x7F,0x14,0x7F,0x14}, // #
    {0x63,0x13,0x08,0x64,0x63}, // %
    {0x00,0x08,0x08,0x08,0x00}  // _
};

static void SSD1306_WriteCommand(uint8_t command)
{
    uint8_t data[2];

    data[0] = 0x00;
    data[1] = command;

    HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, data, 2, HAL_MAX_DELAY);
}

static void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    {
        return;
    }

    if (color == White)
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
    }
    else
    {
        SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_Clear(void)
{
    memset(SSD1306_Buffer, 0x00, sizeof(SSD1306_Buffer));
}

void SSD1306_UpdateScreen(void)
{
    uint8_t data[129];

    for (uint8_t i = 0; i < 8; i++)
    {
        SSD1306_WriteCommand(0xB0 + i);
        SSD1306_WriteCommand(0x00);
        SSD1306_WriteCommand(0x10);

        data[0] = 0x40;

        memcpy(&data[1], &SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);

        HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, data, 129, HAL_MAX_DELAY);
    }
}

void SSD1306_Init(void)
{
    HAL_Delay(100);

    SSD1306_WriteCommand(0xAE);
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x10);
    SSD1306_WriteCommand(0xB0);
    SSD1306_WriteCommand(0xC8);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0x10);
    SSD1306_WriteCommand(0x40);
    SSD1306_WriteCommand(0x81);
    SSD1306_WriteCommand(0xFF);
    SSD1306_WriteCommand(0xA1);
    SSD1306_WriteCommand(0xA6);
    SSD1306_WriteCommand(0xA8);
    SSD1306_WriteCommand(0x3F);
    SSD1306_WriteCommand(0xA4);
    SSD1306_WriteCommand(0xD3);
    SSD1306_WriteCommand(0x00);
    SSD1306_WriteCommand(0xD5);
    SSD1306_WriteCommand(0xF0);
    SSD1306_WriteCommand(0xD9);
    SSD1306_WriteCommand(0x22);
    SSD1306_WriteCommand(0xDA);
    SSD1306_WriteCommand(0x12);
    SSD1306_WriteCommand(0xDB);
    SSD1306_WriteCommand(0x20);
    SSD1306_WriteCommand(0x8D);
    SSD1306_WriteCommand(0x14);
    SSD1306_WriteCommand(0xAF);

    SSD1306_Clear();
    SSD1306_UpdateScreen();

    CurrentX = 0;
    CurrentY = 0;
}

void SSD1306_GotoXY(uint8_t x, uint8_t y)
{
    CurrentX = x;
    CurrentY = y;
}

static uint8_t SSD1306_GetCharIndex(char ch)
{
    if (ch == ' ')
        return 0;

    if (ch >= '0' && ch <= '9')
        return ch - '0' + 1;

    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A' + 11;

    if (ch >= 'a' && ch <= 'z')
        return ch - 'a' + 11;

    if (ch == ':')
        return 37;

    if (ch == '!')
        return 38;

    if (ch == '-')
        return 39;

    if (ch == '.')
        return 40;

    if (ch == '#')
        return 41;

    if (ch == '%')
        return 42;

    if (ch == '_')
        return 43;

    return 0;
}

static void SSD1306_DrawChar(char ch, SSD1306_COLOR color)
{
    uint8_t index;

    index = SSD1306_GetCharIndex(ch);

    if (CurrentX + 6 >= SSD1306_WIDTH)
    {
        CurrentX = 0;
        CurrentY += 8;
    }

    if (CurrentY + 8 >= SSD1306_HEIGHT)
    {
        return;
    }

    for (uint8_t col = 0; col < 5; col++)
    {
        uint8_t line = Font5x7[index][col];

        for (uint8_t row = 0; row < 7; row++)
        {
            if (line & (1 << row))
            {
                SSD1306_DrawPixel(CurrentX + col, CurrentY + row, color);
            }
            else
            {
                SSD1306_DrawPixel(CurrentX + col, CurrentY + row, Black);
            }
        }
    }

    CurrentX += 6;
}

void SSD1306_Puts(char *str, SSD1306_COLOR color)
{
    while (*str)
    {
        SSD1306_DrawChar(*str, color);
        str++;
    }
}
