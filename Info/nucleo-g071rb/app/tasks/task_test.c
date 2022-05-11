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

//Driver from mpLib
#include "mp/drivers/uart.h"
#include "mp/drivers/adc.h"
#include "mp/drivers/spi.h"
#include "mp/drivers/gpio.h"

//std
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Implemented functions -------------------------------------------------------


#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_exti.h"
volatile int cpt2=0;

void testBpIt()
{
    cpt2++;
}

void task_test( void* pvParameters )
{
    (void)pvParameters;
    
    // Init gpios
    mp_gpio_init(drv_gpio);
    
    //// Init UART1
    //mp_uart_init(drv_uart1);
    //mp_uart_config(drv_uart1, MP_UART_BAUDRATE_115200,
                            //MP_UART_DATA_8BITS,
                            //MP_UART_PARITY_NO,
                            //MP_UART_STOPBIT_1);
    
    //// Init ADC1
    //mp_adc_init(drv_adc1);
    
    //// Init SPI1 to Init ads7822 (adc2)
    //mp_spi_init(drv_spi1);
    //mp_adc_init(drv_adc2);



    
    
    //mp_gpio_set_output(LED_GREEN,    MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    0);
    //mp_gpio_set_input(BP1, MP_GPIO_PULL_NO);
    //mp_gpio_set_input(BP2, MP_GPIO_PULL_UP);
    
    
    
    //mp_gpio_set_output(PIN_LED_RED,    MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    1);
    //mp_gpio_set_output(PIN_LED_GREEN,  MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    1);
    //mp_gpio_set_output(PIN_LED_YELLOW, MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    1);
    
    //mp_gpio_set_input(PIN_BP_LED, MP_GPIO_PULL_UP);
    
    
    
    
    int cpt = 0;
    while(1)
    {
        //vTaskDelay(450 / portTICK_PERIOD_MS);
        LL_mDelay(450);
        mp_gpio_set(PIN_LED_GREEN);
        mp_gpio_toggle(PIN_LED_YELLOW);
        mp_gpio_set(LED_GREEN);
        
        //vTaskDelay(50 / portTICK_PERIOD_MS);
        LL_mDelay(50);
        mp_gpio_reset(PIN_LED_GREEN);
        mp_gpio_toggle(PIN_LED_YELLOW);
        mp_gpio_reset(LED_GREEN);
        
        //mp_uart_printf(drv_uart1, "Hello:%u:%u\r\n", cpt, cpt2);
        cpt++;
        
        
        unsigned int val = mp_gpio_get_value(PIN_BP_LED);
        mp_gpio_set_value(PIN_LED_RED, val);
        
        
        //mp_adc_get_volatag(&drv_adc1, 0);
        //mp_adc_get_volatag(&drv_adc1, 4);
        //mp_adc_get_volatag(&drv_adc1, 6);
        
        //mp_adc_get_volatag(&drv_adc2, 0);
    }
}

void bp1Handler()
{
    cpt2++;
}

void bp2Handler()
{
    cpt2--;
}




