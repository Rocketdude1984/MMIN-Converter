#include "ssd1305.h"
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart1;
const char *newLine = "\n\r";

void displayString(const char *input_buffer)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)input_buffer, strlen(input_buffer), HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t*)newLine, 2, HAL_MAX_DELAY);
}
