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

#ifndef MP_FREERTOS_STM32G0XX_UART_H
#define MP_FREERTOS_STM32G0XX_UART_H

// Include ---------------------------------------------------------------------
#include <stm32g0xx.h>
// No necessary to include "mp/drivers/uart.h" here. It is "mp/drivers/uart.h"
// which include "mp_port_uart.h" after to have declare enum, strucur, typdef, ...
// like mp_uart_baudrate_t, mp_uart_databits_t, mp_uart_stopbit_t ...

// Structure -------------------------------------------------------------------
typedef struct
{
    USART_TypeDef* dev;
    char txBuf[64];
    unsigned int lenSend;
    unsigned int iSend;
}mp_port_uart_t;

// Prototype functions ---------------------------------------------------------
int mp_port_uart_init(mp_port_uart_t *drv,  USART_TypeDef *dev);
int mp_port_uart_deinit(mp_port_uart_t *drv);
int mp_port_uart_config(mp_port_uart_t *drv,
                        mp_uart_baudrate_t baudrate,
                        mp_uart_databits_t databit,
                        mp_uart_parity_t parity,
                        mp_uart_stopbit_t stopbit);
int mp_port_uart_write(mp_port_uart_t *drv, const void *buf, size_t nbyte);
int mp_port_uart_read(mp_port_uart_t *drv, void *buf, size_t nbyte);
int mp_port_uart_ctl(mp_port_uart_t *drv, int request, va_list ap);

#endif // MP_FREERTOS_STM32G0XX_UART_H
