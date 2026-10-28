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
#include <stdio.h>
#include <stdlib.h>
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
/* USER CODE BEGIN PV */

int current = 21;
int player_move = 0;
int ai_move = 0;
int ai_turn = 0;

char str[17];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void LCD_SendNibble(uint8_t nibble, uint8_t rs)
{
    HAL_GPIO_WritePin(
        RS_GPIO_Port,
        RS_Pin,
        rs ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        D4_GPIO_Port,
        D4_Pin,
        (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        D5_GPIO_Port,
        D5_Pin,
        (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        D6_GPIO_Port,
        D6_Pin,
        (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(
        D7_GPIO_Port,
        D7_Pin,
        (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_SendByte(uint8_t byte, uint8_t rs)
{
    LCD_SendNibble(byte >> 4, rs);
    LCD_SendNibble(byte & 0x0F, rs);
}

void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_SendNibble(0x03, 0);
    HAL_Delay(5);

    LCD_SendNibble(0x03, 0);
    HAL_Delay(1);

    LCD_SendNibble(0x03, 0);
    HAL_Delay(1);

    LCD_SendNibble(0x02, 0);
    HAL_Delay(1);

    LCD_SendByte(0x28, 0);
    HAL_Delay(1);

    LCD_SendByte(0x08, 0);
    HAL_Delay(1);

    LCD_SendByte(0x01, 0);
    HAL_Delay(2);

    LCD_SendByte(0x06, 0);
    HAL_Delay(1);

    LCD_SendByte(0x0C, 0);
    HAL_Delay(1);
}

void LCD_Clear(void)
{
    LCD_SendByte(0x01, 0);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t column)
{
    uint8_t address;

    if (row == 0)
    {
        address = column;
    }
    else
    {
        address = 0x40 + column;
    }

    LCD_SendByte(0x80 | address, 0);
}

void LCD_DisplayString(char *str)
{
    while (*str)
    {
        LCD_SendByte((uint8_t)(*str), 1);
        str++;
    }
}

void DisplayPlayerTurn(void)
{
    LCD_Clear();

    snprintf(
        str,
        sizeof(str),
        "%d Your Turn",
        current
    );

    LCD_SetCursor(0, 0);
    LCD_DisplayString(str);

    LCD_SetCursor(1, 0);
    LCD_DisplayString("Hold Button");
}

void DisplayPlayerChoice(int move)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_DisplayString("PICK 1,2,3:");

    snprintf(
        str,
        sizeof(str),
        "%d",
        move
    );

    LCD_SetCursor(1, 0);
    LCD_DisplayString(str);
}

void DisplayAIState(void)
{
    LCD_Clear();

    snprintf(
        str,
        sizeof(str),
        "Current: %d",
        current
    );

    LCD_SetCursor(0, 0);
    LCD_DisplayString(str);

    LCD_SetCursor(1, 0);
    LCD_DisplayString("AI Turn");

    HAL_Delay(1000);
}

void DisplayAIChoice(int move)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_DisplayString("AI Selected");

    snprintf(
        str,
        sizeof(str),
        "%d",
        move
    );

    LCD_SetCursor(1, 0);
    LCD_DisplayString(str);

    HAL_Delay(1000);
}

void DisplayPlayerLose(void)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_DisplayString("You Lose!");

    LCD_SetCursor(1, 0);
    LCD_DisplayString("Game Over");
}

void DisplayPlayerWin(void)
{
    LCD_Clear();

    LCD_SetCursor(0, 0);
    LCD_DisplayString("You Win!");

    LCD_SetCursor(1, 0);
    LCD_DisplayString("AI Loses");
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

  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */

  LCD_Init();
  LCD_Clear();

  LCD_SetCursor(0, 0);
  LCD_DisplayString("Game Start");

  LCD_SetCursor(1, 0);
  LCD_DisplayString("Start = 21");

  HAL_Delay(1500);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    if (ai_turn == 0)
    {
        DisplayPlayerTurn();

        player_move = 0;

        while (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == GPIO_PIN_RESET)
        {
        }

        while (HAL_GPIO_ReadPin(BT_GPIO_Port, BT_Pin) == GPIO_PIN_SET)
        {
            player_move++;

            if (player_move > 3)
            {
                player_move = 1;
            }

            DisplayPlayerChoice(player_move);

            HAL_Delay(500);
        }

        if (player_move > 0)
        {
            current = current - player_move;

            if (current <= 0)
            {
                DisplayPlayerLose();
                break;
            }

            ai_turn = 1;
        }
    }
    else
    {
        DisplayAIState();

        srand(HAL_GetTick());

        ai_move = (rand() % 3) + 1;

        DisplayAIChoice(ai_move);

        current = current - ai_move;

        if (current <= 0)
        {
            DisplayPlayerWin();
            break;
        }

        ai_turn = 0;
    }

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

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(
          &RCC_ClkInitStruct,
          FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(
      GPIOB,
      D4_Pin | D5_Pin,
      GPIO_PIN_RESET
  );

  HAL_GPIO_WritePin(
      GPIOD,
      RS_Pin | D6_Pin | E_Pin | D7_Pin,
      GPIO_PIN_RESET
  );

  GPIO_InitStruct.Pin = BT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BT_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = D4_Pin | D5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =
      RS_Pin |
      D6_Pin |
      E_Pin |
      D7_Pin;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

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

  __disable_irq();

  while (1)
  {
  }

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */

  /* USER CODE END 6 */
}

#endif
