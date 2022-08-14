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

#ifndef MP_UART_PORT_H
#define MP_UART_PORT_H

// Include ---------------------------------------------------------------------
#include <stm32g0xx.h>
// Don't include "mp/drivers/uart.h" here. It is "mp/drivers/uart.h" which
// include "mp_uart_port.h" after to have declare enum, strucur, typdef, ...

// St Low level
#include <stm32g0xx_ll_usart.h>
#include <stm32g0xx_ll_lpuart.h>

// Define macro ----------------------------------------------------------------
#define MP_PORT_UART(dev)           ((mp_uart_port_t *)dev)
#define MP_PORT_UART_GET(devid)     MP_PORT_UART(mp_device_get(devid))

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_uart_t uart_parent;
    USART_TypeDef * const uartx;
    
    char txBuf[1024*2];
    unsigned int lenSend;
    unsigned int iSend;
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


// Static inline ISR -----------------------------------------------------------
__attribute__((always_inline))
static inline void mp_uart_port_usartx_isr(mp_uart_port_t * dev)
{
    USART_TypeDef * uartx = dev->uartx;
    
    if (LL_USART_IsEnabledIT_TXE_TXFNF(uartx) && LL_USART_IsActiveFlag_TXE_TXFNF(uartx))
    {
        /* TXE flag will be automatically cleared when writing new data in TDR register */
    
        /* Call function in charge of handling empty DR => will lead to transmission of next character */
        //USART_TXEmpty_Callback();
        
        if (dev->iSend >= dev->lenSend -1)
        {
            /* Disable TXE interrupt */
            LL_USART_DisableIT_TXE_TXFNF(uartx);
          
            ///* Enable TC interrupt */
            //LL_USART_EnableIT_TC(uartx);
        }

        /* Fill TDR with a new char */
        LL_USART_TransmitData8(uartx, dev->txBuf[dev->iSend]);
        dev->iSend++;
    }
    
    //if (LL_USART_IsEnabledIT_TC(uartx) && LL_USART_IsActiveFlag_TC(uartx))
    //{
        ///* Clear TC flag */
        //LL_USART_ClearFlag_TC(uartx);
        ///* Call function in charge of handling end of transmission of sent character
        //and prepare next character transmission */
        ////USART_CharTransmitComplete_Callback();
        
        ///* Disable TC interrupt */
        //LL_USART_DisableIT_TC(uartx);
    //}
    
    //if (LL_USART_IsEnabledIT_ERROR(uartx) && LL_USART_IsActiveFlag_NE(uartx))
    //{
        /* Call Error function */
        //Error_Callback();
        
        /* Disable USARTx_IRQn */
        //NVIC_DisableIRQ(USART2_IRQn);
        //mp_port_irq_disable(usart);
    //}
}

__attribute__((always_inline))
static inline void mp_uart_port_usart1_isr()
{
    mp_uart_port_usartx_isr(_mp_uart_port_usart1_dev);
}

__attribute__((always_inline))
static inline void mp_uart_port_usart2_isr()
{
    mp_uart_port_usartx_isr(_mp_uart_port_usart2_dev);
}

#endif // MP_UART_PORT_H
