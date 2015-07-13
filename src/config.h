/*
 * config.h
 *
 *  Created on: Mar 18, 2015
 *      Author: iwasz
 */

#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

extern void Error_Handler (void);

/*---------------------------------------------------------------------------*/
/* CDC ACM STUFF                                                             */
/*---------------------------------------------------------------------------*/

/* Definition for TIM_CDC clock resources */
#define TIM_CDC                             TIM3
#define TIM_CDC_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
#define TIM_CDC_FORCE_RESET()               __HAL_RCC_USART3_FORCE_RESET()
#define TIM_CDC_RELEASE_RESET()             __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for TIM_CDC's NVIC */
#define TIM_CDC_IRQn                        TIM3_IRQn
#define TIM_CDC_IRQHandler                  TIM3_IRQHandler

#define CDC_RX_DATA_SIZE  1024
#define CDC_TX_DATA_SIZE  1024

/*---------------------------------------------------------------------------*/
/* Diody do debugu                                                           */
/*---------------------------------------------------------------------------*/

// D2 PB7
#define __HAL_RCC_LED_D2_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#define GPIO_PIN_LED_D2 GPIO_PIN_12
#define GPIO_LED_D2 GPIOD

// D3 PB6
#define __HAL_RCC_LED_D3_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#define GPIO_PIN_LED_D3 GPIO_PIN_13
#define GPIO_LED_D3 GPIOD

// D4 PB5
#define __HAL_RCC_LED_D4_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#define GPIO_PIN_LED_D4 GPIO_PIN_14
#define GPIO_LED_D4 GPIOD

// D5 PB4
#define __HAL_RCC_LED_D5_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#define GPIO_PIN_LED_D5 GPIO_PIN_15
#define GPIO_LED_D5 GPIOD

#endif /* SRC_CONFIG_H_ */
