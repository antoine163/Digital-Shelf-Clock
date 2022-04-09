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
#include "mp/drivers/uart.h"

#include <stm32g0xx_ll_usart.h>
#include <stm32g0xx_ll_lpuart.h>

// Implemented functions -------------------------------------------------------
int mp_port_uart_init(mp_port_uart_t *drv,  USART_TypeDef *dev)
{
    drv->dev = dev;
    return -1;
}

//int mp_uart_deinit(mp_uart_t *inc)
//{
    //(void)inc;
    
    //return 0;
//}

//int mp_uart_config(mp_uart_t *inc,  mp_uart_baudrate_t baudrate,
                                    //mp_uart_databits_t databit,
                                    //mp_uart_parity_t parity,
                                    //mp_uart_stopbit_t stopbit)
//{
    //(void)inc;
    //(void)baudrate;
    //(void)databit;
    //(void)parity;
    //(void)stopbit;
    
    //return 0;
//}

//int mp_uart_write(mp_uart_t *inc, const void *buf, size_t nbyte)
//{
    //(void)inc;
    //(void)buf;
    //(void)nbyte;
    
    //return 0;
//}

//int mp_uart_read(mp_uart_t *inc, void *buf, size_t nbyte)
//{
    //(void)inc;
    //(void)buf;
    //(void)nbyte;
    
    //return 0;
//}

//int mp_uart_ctl(mp_uart_t *inc, int request, ...)
//{
    //(void)inc;
    //(void)request;
    
    //return 0;
//}
