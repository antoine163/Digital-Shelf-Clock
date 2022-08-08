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
//#include "FreeRTOS.h"
//#include "task.h"

//Driver from mpLib
//#include "mp/drivers/uart.h"
//#include "mp/drivers/adc.h"
//#include "mp/drivers/spi.h"
#include "mp/drivers/gpio.h"
#include "mp/drivers/interrupt.h"

//std
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Implemented functions -------------------------------------------------------


#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_exti.h"
volatile int cpt2=0;

void bp1Handler(mp_gpio_trigger_t);
void bp2Handler(mp_gpio_trigger_t);

void bpLedHandler(mp_gpio_trigger_t);

unsigned char digit[] = {0xc0,   //0
                        0xf9,   //1
                        0xa4,   //2
                        0xb0,   //3
                        0x99,   //4
                        0x92,   //5
                        0x82,   //6
                        0xf8,   //7
                        0x80,   //8
                        0x90,   //9
                        0x88,   //a
                        0x83,   //b
                        0xc6,   //c
                        0xa1,   //d
                        0x86,   //e
                        0x8e,   //f
                        0x40,   //0,
                        0x79,   //1,
                        0x24,   //2,
                        0x30,   //3,
                        0x19,   //4,
                        0x12,   //5,
                        0x02,   //6,
                        0x78,   //7,
                        0x00,   //8,
                        0x10,   //9,
                        0xcf,   //|
                        0xf9,   // |
                        0xf7,   //_
                        0xfe,   //_ //up
                        0xbf,   //-
                        0xff};  //
                        

void task_test( void* pvParameters )
{
    (void)pvParameters;
    
    // Init gpios
    mp_gpio_init(dev_gpioa);
    mp_gpio_init(dev_gpiob);
    mp_gpio_init(dev_gpioc);
    mp_gpio_init(dev_gpiod);
    mp_gpio_init(dev_gpiof);
    
    /* GPIO OUT:    name,           type,   pull,   default level             */
    MP_GPIO_OUT(LED_GREEN,      PUSH_PULL,  NO,     1);
    MP_GPIO_OUT(PIN_LED_RED,    PUSH_PULL,  NO,     1);
    MP_GPIO_OUT(PIN_LED_GREEN,  PUSH_PULL,  NO,     HIGH);
    MP_GPIO_OUT(PIN_LED_YELLOW, PUSH_PULL,  NO,     HIGH);
    /* AFF_7SEG                                                               */
    MP_GPIO_OUT(AFF_7SEG,       PUSH_PULL,  NO,     MASK(0x7f));
    /* GPIO IN:     name,                   pull                              */
    MP_GPIO_IN( BP1,                        NO);
    MP_GPIO_IN( BP2,                        UP);
    MP_GPIO_IN( PIN_BP_LED,                 UP);

    
    mp_interrupt_init();
    
    // grep "<mp[^ ]*>:" digital_shelf_clock.lss
    // rm-none-eabi-readelf digital_shelf_clock.elf -wi > /tmp/readelf
    mp_gpio_enableIsr(BP1, MP_GPIO_TRIGGER_FALLING, bp1Handler);
    mp_gpio_enableIsr(BP2, MP_GPIO_TRIGGER_FALLING, bp2Handler);
    mp_gpio_enableIsr(PIN_BP_LED, MP_GPIO_TRIGGER_RISING, bpLedHandler);

    
#if 1
    int cpt = 0;
    while(1)
    {
        //vTaskDelay(450 / portTICK_PERIOD_MS);
        LL_mDelay(450);
        mp_gpio_up(PIN_LED_GREEN);
        mp_gpio_toggle(PIN_LED_YELLOW);
        mp_gpio_up(LED_GREEN);
        
        //vTaskDelay(50 / portTICK_PERIOD_MS);
        LL_mDelay(50);
        mp_gpio_down(PIN_LED_GREEN);
        mp_gpio_toggle(PIN_LED_YELLOW);
        mp_gpio_down(LED_GREEN);
        
        //mp_uart_printf(drv_uart1, "Hello:%u:%u\r\n", cpt, cpt2);
        cpt++;
        if(cpt >= (int)sizeof(digit))
            cpt = 0;
        
        
        unsigned int val = mp_gpio_getValue(PIN_BP_LED);
        mp_gpio_setValue(PIN_LED_RED, val);
        
        //mp_gpio_setLevel(AFF_7SEG, digit[cpt]);
        mp_gpio_setLevel(AFF_7SEG, digit[cpt2]);
        
        
        ////mp_adc_get_volatag(&drv_adc1, 0);
        ////mp_adc_get_volatag(&drv_adc1, 4);
        ////mp_adc_get_volatag(&drv_adc1, 6);
        
        ////mp_adc_get_volatag(&drv_adc2, 0);
    }
#endif
}

void bp1Handler(mp_gpio_trigger_t)
{
    cpt2++;
}

void bp2Handler(mp_gpio_trigger_t)
{
    cpt2--;
    if( cpt2 < 0)
        cpt2 = 0;
}

void bpLedHandler(mp_gpio_trigger_t)
{
    cpt2 = 0;
    if( cpt2 >= (int)sizeof(digit))
        cpt2 = sizeof(digit)-1;
}





