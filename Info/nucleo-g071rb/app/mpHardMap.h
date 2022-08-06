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
 
#ifndef MP_HARD_MAP_H
#define MP_HARD_MAP_H

// Defines ---------------------------------------------------------------------
#define PIN_LED_RED         MP_GPIO_PIN(dev_gpiob, 13)
#define PIN_LED_GREEN       MP_GPIO_PIN(dev_gpiob, 10)
#define PIN_LED_YELLOW      MP_GPIO_PIN(dev_gpiob, 15)
#define PIN_BP_LED          MP_GPIO_PIN(dev_gpiob, 2)

#define LED_GREEN           MP_GPIO_PIN(dev_gpioa, 5)
#define BP1                 MP_GPIO_PIN(dev_gpioc, 13)
#define BP2                 MP_GPIO_PIN(dev_gpiob, 7)

//#define USART2_TX           MP_GPIO_PIN(dev_gpioa, 2)
//#define USART2_RX           MP_GPIO_PIN(dev_gpioa, 3)


#define AFF_7SEG            MP_GPIO_PIN(dev_gpioc, 0, 1, 2, 3, 4, 5, 6, 10)
//#define AFF_7SEG_A          MP_GPIO_PIN(dev_gpioc, 0)
//#define AFF_7SEG_B          MP_GPIO_PIN(dev_gpioc, 1)
//#define AFF_7SEG_C          MP_GPIO_PIN(dev_gpioc, 2)
//#define AFF_7SEG_D          MP_GPIO_PIN(dev_gpioc, 3)
//#define AFF_7SEG_E          MP_GPIO_PIN(dev_gpioc, 4)
//#define AFF_7SEG_F          MP_GPIO_PIN(dev_gpioc, 5)
//#define AFF_7SEG_G          MP_GPIO_PIN(dev_gpioc, 6)
//#define AFF_7SEG_DP         MP_GPIO_PIN(dev_gpioc, 10)

/**
 * @brief Table of GPIO
 * 
 * type :
 * - PUSH_PULL (@ref MP_GPIO_TYPE_PUSH_PULL)
 * - OPEN_DRAIN (@ref MP_GPIO_TYPE_OPEN_DRAIN)
 * - OPEN_SOURCE (@ref MP_GPIO_TYPE_OPEN_SOURCE)
 * 
 * pull :
 * - NO (@ref MP_GPIO_PULL_NO)
 * - PULL_UP (@ref MP_GPIO_PULL_UP)
 * - PULL_DOWN (@ref MP_GPIO_PULL_DOWN)
 * 
 */
#define MP_GPIO_TABLE                                                  \
/* GPIO OUT:    name,           type,       pull,   default level    */\
    MP_GPIO_OUT(LED_GREEN,      PUSH_PULL,  NO,     1)                 \
    MP_GPIO_OUT(PIN_LED_RED,    PUSH_PULL,  NO,     1)                 \
    MP_GPIO_OUT(PIN_LED_GREEN,  PUSH_PULL,  NO,     HIGH)              \
    MP_GPIO_OUT(PIN_LED_YELLOW, PUSH_PULL,  NO,     HIGH)              \
/* AFF_7SEG                                                          */\
    MP_GPIO_OUT(AFF_7SEG,       PUSH_PULL,  NO,     MASK(0x7f))        \
/* GPIO IN:     name,                       pull                     */\
    MP_GPIO_IN( BP1,                        NO)                        \
    MP_GPIO_IN( BP2,                        UP)                        \
    MP_GPIO_IN( PIN_BP_LED,                 UP)



#if 0

#define MP_HANDLER_TABLE                                               \
/*              vector,             priority,   handler(s)           */\
    MP_HANDLER( USART1,             1,          mp_port_uart_handler(USART1))\
    MP_HANDLER( USART3_4_LPUART1,   1,          mp_port_uart_handler(USART3);\
                                                mp_port_uart_handler(USART4);\
                                                mp_port_uart_handler(LPUART1)\
    MP_HANDLER( USART2,             1,          mp_port_uart_handler(USART2))
    MP_HANDLER( EXTI0_1,            1,          mp_gpio_port_0_1_handler())
    MP_HANDLER( EXTI2_3,            1,          mp_gpio_port_2_3_handler())
    MP_HANDLER( EXTI4_15,           1,          mp_gpio_port_4_15_handler())
                                                
#endif

#endif // MP_HARD_MAP_H
