#ifndef SSD1305_H
#define SSD1305_H

#include <string.h>
#include <stdio.h>
#include "types.h"
#include "constants.h"

void OLED_Select(void);
void OLED_Unselect(void);
void OLED_Command(uint8_t cmd);
void OLED_Data(const uint8_t *data, uint16_t size);
void OLED_Clear();
void OLED_Reset(void);
void OLED_Init(void);
void drawChar5x7(int8_t x, char c);
void drawString5x7(const char *str);
void drawChar8x16(int8_t x, char c);
void drawString8x16(const char *str);
void drawCursor();
void eraseCursor();
void splashScreen();

#endif
