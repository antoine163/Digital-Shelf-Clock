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

#define AFF_7SEG            MP_GPIO_PIN(dev_gpioc, 0, 1, 2, 3, 4, 5, 6, 10)


// Tick
#define MP_TICK_CLOCK_HZ            ( SystemCoreClock )
#define MP_TICK_RATE_HZ             ( 1000 )


// Pins of USART1 ( dev_ws2812b )
#define MP_USART1_TX_GPIO_Port      GPIOA
#define MP_USART1_TX_Pin            LL_GPIO_PIN_9
#define MP_USART1_TX_AF             LL_GPIO_AF_1
#define MP_USART1_TX_SPEED          LL_GPIO_SPEED_FREQ_HIGH
#define MP_USART1_TX_PULL           LL_GPIO_PULL_UP
#define MP_USART1_TX_OUTPUT         LL_GPIO_OUTPUT_PUSHPULL
#define MP_USART1_TX_PIN_LEVEL      LL_USART_TXPIN_LEVEL_STANDARD
#define MP_USART1_TX_FIFO_SIZE      ( 128 ) // Si 0 ou pas définie la fonction write n'a plus d'effet

#define MP_USART1_CLKSOURCE         LL_RCC_USART1_CLKSOURCE_PCLK1
#define MP_USART1_TXRX_SWAPPED      LL_USART_TXRX_STANDARD



// Pins of USART2 ( dev_tty )
#define MP_USART2_TX_GPIO_Port      GPIOA
#define MP_USART2_TX_Pin            LL_GPIO_PIN_2
#define MP_USART2_TX_AF             LL_GPIO_AF_1
#define MP_USART2_TX_SPEED          LL_GPIO_SPEED_FREQ_HIGH
#define MP_USART2_TX_PULL           LL_GPIO_PULL_UP
#define MP_USART2_TX_OUTPUT         LL_GPIO_OUTPUT_PUSHPULL
#define MP_USART2_TX_PIN_LEVEL      LL_USART_TXPIN_LEVEL_STANDARD
#define MP_USART2_TX_FIFO_SIZE      64

#define MP_USART2_RX_GPIO_Port      GPIOA
#define MP_USART2_RX_Pin            LL_GPIO_PIN_3
#define MP_USART2_RX_SPEED          LL_GPIO_SPEED_FREQ_HIGH
#define MP_USART2_RX_AF             LL_GPIO_AF_1
#define MP_USART2_RX_PULL           LL_GPIO_PULL_UP
#define MP_USART2_RX_PIN_LEVEL      LL_USART_RXPIN_LEVEL_STANDARD
#define MP_USART2_RX_FIFO_SIZE      64

#define MP_USART2_CLKSOURCE         LL_RCC_USART2_CLKSOURCE_PCLK1
#define MP_USART2_TXRX_SWAPPED      LL_USART_TXRX_STANDARD



#define MP_INTERRUPT_TABLE                                             \
    /*          vector,   priority, irq                              */\
    MP_INT_ISR( USART1,   1,        mp_uart_port_usart1_isr())         \
    MP_INT_ISR( USART2,   2,        mp_uart_port_usart2_isr())         \
    MP_INT_ISR( EXTI0_1,  2,        mp_gpio_port_exti0_1_isr())        \
    MP_INT_ISR( EXTI2_3,  2,        mp_gpio_port_exti2_3_isr())        \
    MP_INT_ISR( EXTI4_15, 2,        mp_gpio_port_exti4_15_isr())
    

#endif // MP_HARD_MAP_H
