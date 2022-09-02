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

#ifndef MP_UART_PORT_H
#define MP_UART_PORT_H

// Include ---------------------------------------------------------------------
#include <stm32g0xx.h>
// Don't include "mp/drivers/uart.h" here. It is "mp/drivers/uart.h" which
// include "mp_uart_port.h" after to have declare enum, strucur, typdef, ...

// St Low level
#include <stm32g0xx_ll_usart.h>
#include <stm32g0xx_ll_lpuart.h>

// mpLib
#include "mp/utils/fifo.h"

// Define macro ----------------------------------------------------------------
#define MP_PORT_UART(dev)           ((mp_uart_port_t *)dev)
#define MP_PORT_UART_GET(devid)     MP_PORT_UART(mp_device_get(devid))

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_uart_t uart_parent;
    USART_TypeDef * const uartx;
    mp_fifo_t * fifoRx;
    mp_fifo_t * fifoTx;
}mp_uart_port_t;

// Extern protected global variables -------------------------------------------
extern mp_uart_port_t * _mp_uart_port_usart1_dev;
extern mp_uart_port_t * _mp_uart_port_usart2_dev;
extern mp_uart_port_t * _mp_uart_port_usart3_dev;
extern mp_uart_port_t * _mp_uart_port_usart4_dev;
extern mp_uart_port_t * _mp_uart_port_usart5_dev;
extern mp_uart_port_t * _mp_uart_port_usart6_dev;
extern mp_uart_port_t * _mp_uart_port_lpuart1_dev;
extern mp_uart_port_t * _mp_uart_port_lpuart2_dev;

// Prototype functions ---------------------------------------------------------
int mp_uart_port_init(mp_device_id_t devid);
int mp_uart_port_deinit(mp_device_id_t devid);
int mp_uart_port_config(mp_device_id_t devid,
                        mp_uart_baudrate_t baudrate, mp_uart_databits_t databit,
                        mp_uart_parity_t parity, mp_uart_stopbit_t stopbit);
int mp_uart_port_write(mp_device_id_t devid, void const * buf, size_t nbyte);
int mp_uart_port_read(mp_device_id_t devid, void * buf, size_t nbyte);
int mp_uart_port_ctl(mp_device_id_t devid, int request, va_list ap);

//int mp_uart_port_flush(mp_device_id_t devid); ...

#include "mp/drivers/gpio.h"
// Static inline ISR -----------------------------------------------------------
__attribute__((always_inline))
static inline void mp_uart_port_usartx_fifo_isr(mp_uart_port_t * dev)
{
    USART_TypeDef * uartx = dev->uartx;

    // The USART Tx FiFo is empty ?
    if (    LL_USART_IsEnabledIT_TXFE(uartx)    &&
            LL_USART_IsActiveFlag_TXFE(uartx))
    {
        // Fill TDR with a new bytes from the data Tx FiFo than the USART Tx
        // FiFo is not full and the data Tx FiFo is not empty.
        do
        {
            uint8_t byte;
            MP_FIFO_POP_BYTE(dev->fifoTx, &byte);
            LL_USART_TransmitData8(uartx, byte);
        }
        while(  LL_USART_IsActiveFlag_TXE_TXFNF(uartx) &&
                !mp_fifo_isEmpty(dev->fifoTx));
        
        // Disable USART Tx FiFo empty interrupt if the data Tx FiFo is empty.
        if (mp_fifo_isEmpty(dev->fifoTx))
            LL_USART_DisableIT_TXFE(uartx);
    }
}

__attribute__((always_inline))
static inline void mp_uart_port_usartx_isr(mp_uart_port_t * dev)
{
    USART_TypeDef * uartx = dev->uartx;

    // The USART Transmit Data Register is empty ?
    if (    LL_USART_IsEnabledIT_TXE_TXFNF(uartx)   &&
            LL_USART_IsActiveFlag_TXE_TXFNF(uartx))
    {
        // Fill TDR with a new byte from Tx FiFo.
        uint8_t byte;
        MP_FIFO_POP_BYTE(dev->fifoTx, &byte);
        LL_USART_TransmitData8(uartx, byte);
        
        // Disable TXE interrupt if Tx FiFo in empty.
        if (mp_fifo_isEmpty(dev->fifoTx))
            LL_USART_DisableIT_TXE_TXFNF(uartx);
    }
}

__attribute__((always_inline))
static inline void mp_uart_port_usart1_isr()
{
mp_gpio_up(PIN_LED_RED);
    if (IS_UART_FIFO_INSTANCE(USART1))
        mp_uart_port_usartx_fifo_isr(_mp_uart_port_usart1_dev);
    else
        mp_uart_port_usartx_isr(_mp_uart_port_usart1_dev);
mp_gpio_down(PIN_LED_RED);
}

__attribute__((always_inline))
static inline void mp_uart_port_usart2_isr()
{
mp_gpio_up(PIN_LED_GREEN);
    if (IS_UART_FIFO_INSTANCE(USART2))
        mp_uart_port_usartx_fifo_isr(_mp_uart_port_usart2_dev);
    else
        mp_uart_port_usartx_isr(_mp_uart_port_usart2_dev);
    
mp_gpio_down(PIN_LED_GREEN);
}

#endif // MP_UART_PORT_H
