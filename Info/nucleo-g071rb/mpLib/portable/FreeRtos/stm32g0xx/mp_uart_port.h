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

// St Low level
#include <stm32g0xx_ll_usart.h>
#include <stm32g0xx_ll_lpuart.h>

// Structure -------------------------------------------------------------------
typedef struct
{
    USART_TypeDef* dev;
    char txBuf[64];
    unsigned int lenSend;
    unsigned int iSend;
}mp_port_uart_t;

// Extern protected global variables --------------------------------------------------
extern mp_port_uart_t *_mp_port_uart_usart1_dev;
extern mp_port_uart_t *_mp_port_uart_usart2_dev;
extern mp_port_uart_t *_mp_port_uart_usart3_dev;
extern mp_port_uart_t *_mp_port_uart_usart4_dev;
extern mp_port_uart_t *_mp_port_uart_usart5_dev;
extern mp_port_uart_t *_mp_port_uart_usart6_dev;
extern mp_port_uart_t *_mp_port_uart_lpuart1_dev;
extern mp_port_uart_t *_mp_port_uart_lpuart2_dev;

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


//static inline void mp_port_uart_irq_handler(mp_port_uart_t *drv, USART_TypeDef *dev)
static inline void mp_port_uart_handler(USART_TypeDef *dev)
{
    mp_port_uart_t *drv = NULL;
    
    switch((uint32_t)dev)
    {
        #ifdef USART1
        case (uint32_t)USART1: drv = _mp_stm_usart1_drv; break;
        #endif
        #ifdef USART2
        case (uint32_t)USART2: drv = _mp_stm_usart2_drv; break;
        #endif
        
        //default: assert(); break;
    }
    
    //...
    
    
    //if (0){}
    //#ifdef USART1
    //else if (dev == USART1)
        //drv = _mp_stm_usart1_drv;
    //#endif
    
     //#ifdef USART2
    //else if (dev == USART2)
        //drv = _mp_stm_usart2_drv;
    //#endif
    
     //#ifdef USART3
    //else if (dev == USART3)
        //drv = _mp_stm_usart3_drv;
    //#endif
    
     //#ifdef USART4
    //else if (dev == USART4)
        //drv = _mp_stm_usart4_drv;
    //#endif
    
     //#ifdef USART5
    //else if (dev == USART5)
        //drv = _mp_stm_usart5_drv;
    //#endif
    
     //#ifdef USART6
    //else if (dev == USART6)
        //drv = _mp_stm_usart6_drv;
    //#endif
    
     //#ifdef LPUART1
    //else if (dev == LPUART1)
        //drv = _mp_stm_lpuart1_drv;
    //#endif
    
     //#ifdef LPUART2
    //else if (dev == LPUART2)
        //drv = _mp_stm_lpuart2_drv;
    //#endif
    
    if (LL_USART_IsEnabledIT_TXE_TXFNF(dev) && LL_USART_IsActiveFlag_TXE_TXFNF(dev))
    {
        /* TXE flag will be automatically cleared when writing new data in TDR register */
    
        /* Call function in charge of handling empty DR => will lead to transmission of next character */
        //USART_TXEmpty_Callback();
        
        if (drv->iSend >= drv->lenSend -1)
        {
            /* Disable TXE interrupt */
            LL_USART_DisableIT_TXE_TXFNF(dev);
          
            /* Enable TC interrupt */
            LL_USART_EnableIT_TC(dev);
        }

        /* Fill TDR with a new char */
        LL_USART_TransmitData8(dev, drv->txBuf[drv->iSend]);
        drv->iSend++;
    }
    
    if (LL_USART_IsEnabledIT_TC(dev) && LL_USART_IsActiveFlag_TC(dev))
    {
        /* Clear TC flag */
        LL_USART_ClearFlag_TC(dev);
        /* Call function in charge of handling end of transmission of sent character
        and prepare next character transmission */
        //USART_CharTransmitComplete_Callback();
        
        /* Disable TC interrupt */
        LL_USART_DisableIT_TC(dev);
    }
    
    if (LL_USART_IsEnabledIT_ERROR(dev) && LL_USART_IsActiveFlag_NE(dev))
    {
        /* Call Error function */
        //Error_Callback();
        
        /* Disable USARTx_IRQn */
        //NVIC_DisableIRQ(USART2_IRQn);
        mp_port_irq_disable(drv);
    }
}

#endif // MP_FREERTOS_STM32G0XX_UART_H
