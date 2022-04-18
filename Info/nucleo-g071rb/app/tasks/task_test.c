/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2022 Antoine Maleyrie
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Include ---------------------------------------------------------------------
// App
#include "task_test.h"
#include "board/board.h"

// FreeRtos
#include "FreeRTOS.h"
#include "task.h"

//Driver from mp library
#include "mp/drivers/uart.h"
#include "mp/drivers/adc.h"
#include "mp/drivers/spi.h"

//std
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Implemented functions -------------------------------------------------------

void task_test( void* pvParameters )
{
    (void)pvParameters;
    
    // Init UART1
    mp_uart_init(drv_uart1);
    mp_uart_config(drv_uart1, MP_UART_BAUDRATE_115200,
                            MP_UART_DATA_8BITS,
                            MP_UART_PARITY_NO,
                            MP_UART_STOPBIT_1);
    
    // Init ADC1
    mp_adc_init(drv_adc1);
    
    // Init SPI1 to Init ads7822 (adc2)
    mp_spi_init(drv_spi1);
    mp_adc_init(drv_adc2);
    
    
    int cpt = 0;
    while(1)
    {
        vTaskDelay(450 / portTICK_PERIOD_MS);
        //mp_gpio_set(drv_gpio, LED_GREEN | LED_RED);
        boardLedGreenOn();
        vTaskDelay(50 / portTICK_PERIOD_MS);
        //mp_gpio_reset(drv_gpio, LED_GREEN);
        boardLedGreenOff();
        
        mp_uart_printf(drv_uart1, "Hello:%u\r\n", cpt);
        cpt++;
        
        //mp_adc_get_volatag(&drv_adc1, 0);
        //mp_adc_get_volatag(&drv_adc1, 4);
        //mp_adc_get_volatag(&drv_adc1, 6);
        
        //mp_adc_get_volatag(&drv_adc2, 0);
    }
}

//void task_test( void* pvParameters )
//{
    //(void)pvParameters;
    
    //mp_uart_t uartLog;
    ////mp_uart_t uartLog2;
    //mp_uart_init(&uartLog, USART2);
    ////mp_uart_init(&uartLog2, USART_VIA_I2C);
    ////mp_uart_init(&uartLog, "/dev/ttyUSB0");
    
    //mp_uart_config(&uartLog,  MP_UART_BAUDRATE_115200,
                                    //MP_UART_DATA_8BITS,
                                    //MP_UART_PARITY_NO,
                                    //MP_UART_STOPBIT_1);
    
    //char myStr[64];
    
    //while(1)
    //{
        //vTaskDelay(500 / portTICK_PERIOD_MS);
        //boardLedGreenToggle();
        
        //snprintf(myStr, sizeof(myStr), "Hello Uart!\n");
        //mp_uart_write(&uartLog, myStr, strlen(myStr));
    //}
//}
