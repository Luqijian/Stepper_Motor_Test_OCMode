/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "tim.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Motor_ENA_Pin|Motor_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PHPin PHPin */
  GPIO_InitStruct.Pin = KEY_1_Pin|KEY_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = Motor_ENA_Pin|Motor_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 2 */
/* Get the key pin electrolic level */
uint8_t KEY_SCAN(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    if (0 == HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
    {
        HAL_Delay(5);
        if (0 == HAL_GPIO_ReadPin(GPIOx, GPIO_Pin))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/* GPIO exti call back function */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case KEY_0_Pin:
    {
        if (KEY_SCAN(KEY_0_GPIO_Port, KEY_0_Pin))
        {
            Pulse_Count += 500;

            HAL_TIM_OC_Start_IT(&htim3, TIM_CHANNEL_1);
        }
    }
    break;

    case KEY_1_Pin:
    {
        if (KEY_SCAN(KEY_1_GPIO_Port, KEY_1_Pin))
        {
            static uint8_t direction = 0;
            direction = !direction;
            Pulse_Count = 0;
            HAL_GPIO_WritePin(Motor_DIR_GPIO_Port, Motor_DIR_Pin, direction);
        }
    }
    break;

    default:
        break;
    }
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
