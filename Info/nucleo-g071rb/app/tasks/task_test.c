/***
 * The MIT License (MIT)
 * 
 * Copyright (c) 2022 antoine163
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

// Utils from mpLib
#include "mp/utils/fifo.h"

// Driver from mpLib
#include "mp/drivers/interrupt.h"
#include "mp/drivers/gpio.h"
#include "mp/drivers/uart.h"
//#include "mp/drivers/adc.h"
//#include "mp/drivers/spi.h"

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
                        
//void ws2812b_reset();
void ws2812b_update();
int ws2812b_update_led = 0;
volatile uint8_t brightness_led = 10;
volatile int32_t write_led_index = 15;
volatile uint32_t write_led_cmpUp = 1;
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
    
    
    
    mp_uart_init(dev_ws2812b);
    mp_uart_config(dev_ws2812b, 2500000, 7, 0, 1);
    //mp_uart_config(dev_ws2812b, 8000000, 8, 0, 1); // Todo: verifier si le 8MHz est possible avec la clock de 'uart ...
    //mp_uart_printf(dev_ws2812b, "dev_ws2812b inisilised !\r\n");
    //ws2812b_reset();
    LL_mDelay(1);
    //ws2812b_update();
    
    mp_uart_init(dev_tty);
    mp_uart_config(dev_tty, 115200, 8, 0, 1);
    mp_uart_printf(dev_tty, "nucleo-g071rb inisilised !\r\n");
    
#if 1
    int cpt = 0;
    while(1)
    {
        //vTaskDelay(450 / portTICK_PERIOD_MS);
        LL_mDelay(450);
        //mp_gpio_up(PIN_LED_GREEN);
        mp_gpio_toggle(PIN_LED_YELLOW);
        mp_gpio_up(LED_GREEN);
        
        //vTaskDelay(50 / portTICK_PERIOD_MS);
        LL_mDelay(50);
        //mp_gpio_down(PIN_LED_GREEN);
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
        
        mp_uart_printf(dev_tty, "cpt:%u\tcpt2:%u", cpt, cpt2);
        LL_mDelay(100);
        mp_uart_printf(dev_tty, "\tbrightness_led:%u\r\n", brightness_led);
        
        
        
        //ws2812b_update();
        //LL_mDelay(8);
        
        
        //if (ws2812b_update_led)
        //{
            //ws2812b_update();
            //ws2812b_update_led = 0;
        //}
        
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
    
    brightness_led++;
    ws2812b_update_led = 1;
}

void bp2Handler(mp_gpio_trigger_t)
{
    cpt2--;
    if( cpt2 < 0)
        cpt2 = 0;
        
    brightness_led--;
    ws2812b_update_led = 1;
}

void bpLedHandler(mp_gpio_trigger_t)
{
    cpt2 = 0;
    if( cpt2 >= (int)sizeof(digit))
        cpt2 = sizeof(digit)-1;
        
    brightness_led = 0;
    ws2812b_update_led = 1;
}


void ws2812b_update()
{
    #define CODE1_0  /* (1) MSB / Stop bit */ 0b1111111 /* (0) LSB / Start bit */
    #define CODE1_1  /* (1) MSB / Stop bit */ 0b1111110 /* (0) LSB / Start bit */
    #define CODE2_0  /* (1) MSB / Stop bit */ 0b1111011 /* (0) LSB / Start bit */
    #define CODE2_1  /* (1) MSB / Stop bit */ 0b1110011 /* (0) LSB / Start bit */
    #define CODE3_0  /* (1) MSB / Stop bit */ 0b1011111 /* (0) LSB / Start bit */
    #define CODE3_1  /* (1) MSB / Stop bit */ 0b0011111 /* (0) LSB / Start bit */
    
    
    #define NB_LED  (3 * 85) // 255
    
    
    uint32_t colorLeds[NB_LED];
    
    for(int i=0; i<NB_LED;)
    {
        colorLeds[i++] = brightness_led << 16; //GRB
        colorLeds[i++] = brightness_led << 8;
        colorLeds[i++] = brightness_led << 0;
    }
    
    
    colorLeds[write_led_index] = 0x00ffffff;
    
    
    if(write_led_cmpUp)
    {
        write_led_index+=1;
        if(write_led_index >= NB_LED)
        {
            write_led_cmpUp = 0;
            write_led_index = NB_LED-2;
        }
    }
    else
    {
        write_led_index-=1;
        if(write_led_index < 0)
        {
            write_led_cmpUp = 1;
            write_led_index = 0;
        }
    }
    
    
    uint8_t color;
    
    for (int iColorLed=0; iColorLed<NB_LED; iColorLed++)
    {
        for (int iByte=23; iByte>=0;)
        {
            color = 0xff;
            
            if (colorLeds[iColorLed] & (1<<iByte))
                color &= CODE1_1;
            else
                color &= CODE1_0;
            iByte--;
            if (colorLeds[iColorLed] & (1<<iByte))
                color &= CODE2_1;
            else
                color &= CODE2_0;
            iByte--;
            if (colorLeds[iColorLed] & (1<<iByte))
                color &= CODE3_1;
            else
                color &= CODE3_0;
            iByte--;

            mp_uart_write(dev_ws2812b, &color, 1);
        }
    }
}

//void ws2812b_test()
//{
    //#define CODE_0  0b11111100
    //#define CODE_1  0b11000000
    
    //uint32_t colorLeds[3];
    
    //colorLeds[0] = brightness_led << 16; //GRB
    //colorLeds[1] = brightness_led << 8;
    //colorLeds[2] = brightness_led << 0;
    
    
    //uint8_t buf[128];
    //int iBuf = 0;
    
    //for(int iColorLed=0; iColorLed<3; iColorLed++)
    //{
        //for(int iByte=23; iByte>=0; iByte--)
        //{
            //if(colorLeds[iColorLed] & (1<<iByte))
            //{
                //buf[iBuf] = CODE_1;
            //}
            //else
            //{
                //buf[iBuf] = CODE_0;
            //}
            //iBuf++;
        //}
    //}
    
    //mp_uart_write(dev_ws2812b, buf, iBuf+1);
//}



