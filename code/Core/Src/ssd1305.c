#include <string.h>
#include "ssd1305.h"
#include "stm32f1xx_hal.h"
#include "font5x7.h"
#include "font8x16.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

void OLED_Select() {
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
}

void OLED_Unselect() {
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void OLED_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(D_C_GPIO_Port, D_C_Pin, GPIO_PIN_RESET);
    OLED_Select();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    OLED_Unselect();
}

void OLED_Data(const uint8_t *data, uint16_t size) {
    // 1. Set D/C High for DATA mode
    HAL_GPIO_WritePin(D_C_GPIO_Port, D_C_Pin, GPIO_PIN_SET);
    // 2. Pull CS Low
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
    while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_BSY) == SET);
    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

void OLED_Reset(void)
{
    HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET);
    HAL_Delay(5);
}

void OLED_Init(void)
{
	HAL_GPIO_WritePin(VCC_EN_GPIO_Port, VCC_EN_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
	OLED_Reset();
	HAL_GPIO_WritePin(VCC_EN_GPIO_Port, VCC_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

	OLED_Command(0xAE); // Display Off
	OLED_Command(0xFD); OLED_Command(0x12); // UNLOCK
	OLED_Command(0x20); OLED_Command(0x00);
	OLED_Command(0xB0);
	OLED_Command(0x00);
	OLED_Command(0x10);

	OLED_Command(0xD5); OLED_Command(0xF0); // Clock
	OLED_Command(0xD3); OLED_Command(0x00); // Offset 0
	OLED_Command(0xA8); OLED_Command(0x3F); // Mux 32
	//OLED_Command(0xD8); OLED_Command(0x05);
	OLED_Command(0x40);                     // Start Line 0
	OLED_Command(0xA0);                     // Segment Remap
	OLED_Command(0xC0);                     // COM Scan
	OLED_Command(0xDA); OLED_Command(0x12); // COM Pins (Try 0x02 if fails)
	OLED_Command(0xAD); OLED_Command(0x8E); // External VCC Select
	//OLED_Command(0x81); OLED_Command(0x32); // Contrast
	//OLED_Command(0xD9); OLED_Command(0xF1); // Pre-charge
	//OLED_Command(0xDB); OLED_Command(0x00); // VCOMH Low
	OLED_Command(0xA4);
	OLED_Command(0xA6);
	HAL_Delay(10);
	//OLED_Command(0x2F);
	OLED_Command(0xAF);                     // DISPLAY ON
}

void OLED_Clear(void) {
    for (uint8_t page = 0; page < 4; page++) {
        // 1. Set the Page Address (0xB0, 0xB1, 0xB2, 0xB3)
        OLED_Command(0xB0 + page);

        // 2. Set Column Address to the start (0)
        OLED_Command(0x00); // Low nibble
        OLED_Command(0x10); // High nibble
        uint8_t clearData[128] = {0};
        OLED_Data(clearData, 128); // DC Pin HIGH for Data

    }
}

void drawChar5x7(int8_t x, char c)
{
	if (x < 0){
		return;
	}
	const uint8_t *bitmap = &font5x7[c*5];
	OLED_Command(0xB0);
	OLED_Command(x & 0x0F);
	OLED_Command(0x10 | (x >> 4));
	OLED_Data((uint8_t*)bitmap, 5);
	uint8_t space = 0x00;
	OLED_Data(&space, 1);
}

void drawChar8x16(int8_t x, char c)
{
	if (x < 0){
		return;
	}
	if (c < 32 || c > 126) c = ' '; // Simple bounds check

	    uint16_t fontIndex = (c - 32) * 16;

	    // --- Draw TOP HALF (Page n) ---
	    OLED_Command(0xB1);           // Set Page Start Address
	    OLED_Command(0x00 + (x & 0x0F));     // Set Lower Column Address
	    OLED_Command(0x10 + ((x >> 4) & 0x0F)); // Set Higher Column Address

	    // Send 8 bytes for the top half
	    OLED_Data(&font8x16[fontIndex], 8);

	    // --- Draw BOTTOM HALF (Page n + 1) ---
	    OLED_Command(0xB2);       // Move to next page
	    OLED_Command(0x00 + (x & 0x0F));     // Reset x-coordinate
	    OLED_Command(0x10 + ((x >> 4) & 0x0F));

	    // Send 8 bytes for the bottom half (offset by 8 in the array)
	    OLED_Data(&font8x16[fontIndex + 8], 8);
}

void drawString8x16(const char *str)
{

	int8_t x = 123 - strlen(str)*8;

	while(*str)
	{
		drawChar8x16(x, *str);
		x += 8;
		str++;
	}
}

void drawString5x7(const char *str)
{
	int8_t x = 1;
	if (strlen(str)*6 > 128)
	{
		x = 125 - strlen(str) * 6;
	}

	while(*str)
	{
		drawChar5x7(x, *str);
		x += 6;
		str++;
	}
}

void drawCursor()
{
	drawChar8x16(123,'|');
}

void eraseCursor()
{
	drawChar8x16(123, ' ');
}

void splashScreen()
{
	drawString5x7("MINN Converter V1");
}

