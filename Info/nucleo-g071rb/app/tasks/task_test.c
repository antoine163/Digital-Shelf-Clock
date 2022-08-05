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

//std
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// Implemented functions -------------------------------------------------------


#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_exti.h"
volatile int cpt2=0;

//extern mp_port_gpio_t _dev_gpioc; a supprimer dans gpio.h
//extern mp_port_gpio_t _dev_gpioc;


void bp1Handler(mp_gpio_trigger_t);
void bp2Handler(mp_gpio_trigger_t);

void bp3Handler(mp_gpio_trigger_t);
void bp4Handler(mp_gpio_trigger_t);

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
    //mp_gpio_init(dev_gpioa);
    //mp_gpio_init(dev_gpiob);
    //mp_gpio_init(dev_gpioc);
    //mp_gpio_init(dev_gpiod);
    //mp_gpio_init(dev_gpiof);
    
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

  
    
    //mp_gpio_output(PIN_LED_RED,    MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    MP_GPIO_LEVEL_1);
    //mp_gpio_output(PIN_LED_GREEN,  MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    MP_GPIO_LEVEL_1);
    //mp_gpio_output(PIN_LED_YELLOW, MP_GPIO_TYPE_PUSH_PULL, MP_GPIO_PULL_NO,    MP_GPIO_LEVEL_1);
    
    //mp_gpio_input(PIN_BP_LED, MP_GPIO_PULL_UP);
    
    //volatile mp_device_id_t devid = dev_gpioc;
    mp_device_id_t devid = dev_gpioc;
    
    
    //grep -e "<mp[^ ]*>:" digital_shelf_clock.lss
    //asm("BKPT #0");
    //mp_gpio_enableIt(BP1, MP_GPIO_TRIGGER_RISING, bp1Handler);
    //mp_gpio_enableIt(MP_DEVICE_ID_GPIO_port_dev_gpioc, 1<<13, MP_GPIO_TRIGGER_RISING, bp1Handler);
    //mpfc_gpio_enableIt(dev_gpioc->device_parent.id_device, 1<<13, MP_GPIO_TRIGGER_RISING, bp1Handler);
    //mp_port_gpio_enableIt(devid, 1<<13, MP_GPIO_TRIGGER_RISING, bp1Handler);
    //mp_gpio_enableIt(devid, 1<<13, MP_GPIO_TRIGGER_RISING, bp1Handler);
    //mp_gpio_enableIt(devid, 1<<14, MP_GPIO_TRIGGER_RISING, bp2Handler);
    //mp_gpio_enableIt(devid, 1<<15, MP_GPIO_TRIGGER_RISING, bp3Handler);
    //mp_gpio_enableIt(devid, 1<<16, MP_GPIO_TRIGGER_RISING, bp4Handler);
    //asm("BKPT #1");
    //mp_gpio_enableIt(BP2, MP_GPIO_TRIGGER_RISING, bp2Handler);
    //mp_gpio_enableIt(MP_DEVICE_ID_GPIO_port_dev_gpiob, 1<<7, MP_GPIO_TRIGGER_RISING, bp2Handler);
    //test_gpio_enableIt(dev_gpiob->device_parent.id_device, 1<<7, MP_GPIO_TRIGGER_RISING, bp2Handler);
    //mpfc_gpio_enableIt(test_id_dev, 1<<7, MP_GPIO_TRIGGER_RISING, bp2Handler);
    //mp_gpio_enableIt(test_id_dev, 1<<7, MP_GPIO_TRIGGER_RISING, bp2Handler);
    //asm("BKPT #2");
    
    //mp_gpio_enableIt(BP1, MP_GPIO_TRIGGER_RISING);
    //mp_gpio_enableIt(BP2, MP_GPIO_TRIGGER_RISING);
    
    //asm("BKPT 0x12");
    ////_mp_devices_table[0]->isInit = 2;
    ////_dev_gpioa.gpio_parent.device_parent.isInit = 2;
    //mp_device_t * dev = mp_device_get(MP_DEVICE_ID_GPIO_port_dev_gpioa);
    //dev->isInit = 2;
    //asm("BKPT 0x13");
    
    //asm("BKPT #0");
    //mp_gpio_up(devid, 1<<13);
    //asm("BKPT #1");
    //mp_gpio_up(devid, 1<<14);
    //asm("BKPT #2");
    
    
    //asm("BKPT #0");
    //LL_GPIO_SetOutputPin(GPIOF, 0x45);
    asm("BKPT #1");
    mp_gpio_up(devid, (1<<10));
    asm("BKPT #2");
#if 0
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
        
        mp_gpio_setLevel(AFF_7SEG, digit[cpt]);
        
        
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
}

void bp3Handler(mp_gpio_trigger_t)
{
}

void bp4Handler(mp_gpio_trigger_t)
{
}




