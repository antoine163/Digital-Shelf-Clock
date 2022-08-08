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




#define MP_INTERRUPT_TABLE                                             \
    /*      vector,           priority, handlers                     */\
    MP_INT_ISR( EXTI0_1,      1,        mp_gpio_port_exti0_1_isr())    \
    MP_INT_ISR( EXTI2_3,      1,        mp_gpio_port_exti2_3_isr())    \
    MP_INT_ISR( EXTI4_15,     1,        mp_gpio_port_exti4_15_isr())
    
    
#if 0

#define MP_NVIC_ISR_TABLE                                              \
    /*      vector,           priority, handlers                     */\
    MP_ISR( USART1,           1,        mp_port_uart_isr(USART1))      \
    MP_ISR( USART3_4_LPUART1, 1,        mp_port_uart_isr(USART3);      \
                                        mp_port_uart_isr(USART4);      \
                                        mp_port_uart_isr(LPUART1)      \
    MP_ISR( USART2,           1,        mp_port_uart_isr(USART2))      \
    MP_ISR( EXTI0_1,          1,        mp_gpio_port_exti0_1_isr())    \
    MP_ISR( EXTI2_3,          1,        mp_gpio_port_exti2_3_isr())    \
    MP_ISR( EXTI4_15,         1,        mp_gpio_port_exti4_15_isr())
                                                
#endif

#endif // MP_HARD_MAP_H
