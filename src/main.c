#include <stm32f4xx_hal.h>
#include <stdio.h>
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"
#include "config.h"
#include "debugLed.h"
#include "mma8652/mma8652.h"

USBD_HandleTypeDef USBD_Device;
I2C_HandleTypeDef i2cHandle;

static void SystemClock_Config (void);

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main (void)
{

        /* STM32F4xx HAL library initialization:
         - Configure the Flash prefetch, Flash preread and Buffer caches
         - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
         - Low Level Initialization
         */
        HAL_Init ();

        /* Configure the System clock to 180 MHz */
        SystemClock_Config ();

        /* Init Device Library */
        USBD_Init (&USBD_Device, &VCP_Desc, 0);

        /* Add Supported Class */
        USBD_RegisterClass (&USBD_Device, USBD_CDC_CLASS);

        /* Add CDC Interface Class */
        USBD_CDC_RegisterInterface (&USBD_Device, &USBD_CDC_fops);

        /* Start Device Process */
        USBD_Start (&USBD_Device);

/*---------------------------------------------------------------------------*/

        i2cHandle.Instance = I2Cx;
        i2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        i2cHandle.Init.ClockSpeed = 100000;
        i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        i2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
        i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        i2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
        i2cHandle.Init.OwnAddress1 = 0x2d;
        i2cHandle.Init.OwnAddress2 = 0xFE;

        if (HAL_I2C_Init (&i2cHandle) != HAL_OK) {
                Error_Handler ();
        }

        printf ("MMA8652 Init\n");
        mma8652Init (&i2cHandle);

/*---------------------------------------------------------------------------*/

        uint8_t txBuffer[3];
        uint8_t rxBuffer[6];

        debugLedInit (D2);
        debugLedInit (D3);
        debugLedInit (D4);
        debugLedInit (D5);



        /* Infinite loop */
        while (1) {
//                printf ("Hello world\n");
                debugLedToggle (D2);
                HAL_Delay (1000);


                /* Timeout is set to 10S */
                while (HAL_I2C_Master_Receive (&i2cHandle, (uint8_t) MMA8652_I2C_ADDR, (uint8_t *) rxBuffer, 6, 1000) != HAL_OK) {
                        if (HAL_I2C_GetError (&i2cHandle) != HAL_I2C_ERROR_AF) {
                                Error_Handler ();
                        }
                }

                int16_t x = rxBuffer[0] << 8 | rxBuffer[1];
                int16_t y = rxBuffer[2] << 8 | rxBuffer[3];
                int16_t z = rxBuffer[4] << 8 | rxBuffer[5];

                printf ("%d %d %d\n", x, y, z);

        }
}

static void SystemClock_Config (void)
{
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_OscInitTypeDef RCC_OscInitStruct;

        /* Enable Power Control clock */
        __HAL_RCC_PWR_CLK_ENABLE ();

        /* The voltage scaling allows optimizing the power consumption when the device is
         clocked below the maximum system frequency, to update the voltage scaling value
         regarding system frequency refer to product datasheet.  */
        __HAL_PWR_VOLTAGESCALING_CONFIG (PWR_REGULATOR_VOLTAGE_SCALE1);

        /* Enable HSE Oscillator and activate PLL with HSE as source */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 8;
        RCC_OscInitStruct.PLL.PLLN = 360;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 7;
        if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK) {
                /* Initialization Error */
                Error_Handler ();
        }

//        if (HAL_PWREx_EnableOverDrive () != HAL_OK) {
//                /* Initialization Error */
//                Error_Handler ();
//        }

        /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
         clocks dividers */
        RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
        if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
                /* Initialization Error */
                Error_Handler ();
        }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler (void)
{
        /* User may add here some code to deal with this error */
        while (1) {
        }
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
        /* User can add his own implementation to report the file name and line number,
         ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

        /* Infinite loop */
        while (1)
        {
        }
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
