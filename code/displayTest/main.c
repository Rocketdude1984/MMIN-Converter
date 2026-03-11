/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "font5x7.h"
#include "font8x16.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void OLED_Select(void);
void OLED_Unselect(void);
void OLED_Command(uint8_t cmd);
void OLED_Data(const uint8_t *data, uint16_t size);
void OLED_Reset(void);
void OLED_Init(void);
void drawChar5x7(int8_t x, char c);
void drawString5x7(const char *str);
void drawChar8x16(int8_t x, char c);
void drawString8x16(const char *str);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//extern SPI_HandleTypeDef hspi1;

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
	HAL_Delay(5);
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

	int8_t x = 125 - strlen(str)*8;

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


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();
  drawString5x7("10.0mm+0.021in/61mm-103mm=");
  drawString8x16("12.0987mm");

  //drawString(20, 10, "Hello World!");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);  // or whatever LED pin
	      HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, VCC_EN_Pin|D_C_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CS_Pin|RES_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : VCC_EN_Pin CS_Pin RES_Pin D_C_Pin */
  GPIO_InitStruct.Pin = VCC_EN_Pin|CS_Pin|RES_Pin|D_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
