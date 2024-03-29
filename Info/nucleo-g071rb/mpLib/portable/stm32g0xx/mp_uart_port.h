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
#include "mp_devices.h"

// Define macro ----------------------------------------------------------------
#define MP_PORT_UART(dev)           ((mp_uart_port_t *)dev)
#define MP_PORT_UART_GET(devid)     MP_PORT_UART(mp_device_get(devid))

// Define ----------------------------------------------------------------------
#define MP_UART_PORT_SYNC_TXFIFO_NO_FULL    (1<<0)
#define MP_UART_PORT_SYNC_TX_COMPLETE       (1<<1)
#define MP_UART_PORT_SYNC_RXFIFO_NO_EMPTY   (1<<2)
//#define MP_UART_PORT_SYNC_RXFIFO_FRAME        (1<<3)

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_uart_t uart_parent;
    USART_TypeDef * const uartx;
    
    mp_fifo_t * fifoRx;
    mp_fifo_t * fifoTx;
}mp_uart_port_t;

// Extern protected global variables -------------------------------------------
#ifdef USART1
    extern mp_uart_port_t * _mp_uart_port_usart1_dev;
#endif // USART1

#ifdef USART2
    extern mp_uart_port_t * _mp_uart_port_usart2_dev;
#endif // USART2

#ifdef USART3
    extern mp_uart_port_t * _mp_uart_port_usart3_dev;
#endif // USART3

#ifdef USART4
    extern mp_uart_port_t * _mp_uart_port_usart4_dev;
#endif // USART4

#ifdef USART5
    extern mp_uart_port_t * _mp_uart_port_usart5_dev;
#endif // USART5

#ifdef USART6
    extern mp_uart_port_t * _mp_uart_port_usart6_dev;
#endif // USART6

#ifdef LPUART1
    extern mp_uart_port_t * _mp_uart_port_lpuart1_dev;
#endif // LPUART1

#ifdef LPUART2
    extern mp_uart_port_t * _mp_uart_port_lpuart2_dev;
#endif // LPUART2

// Prototype functions ---------------------------------------------------------
int mp_uart_port_init(mp_device_id_t devid);
int mp_uart_port_deinit(mp_device_id_t devid);
int mp_uart_port_config(mp_device_id_t devid,
                        mp_uart_baudrate_t baudrate, mp_uart_databits_t databit,
                        mp_uart_parity_t parity, mp_uart_stopbit_t stopbit);
int mp_uart_port_ctl(mp_device_id_t devid, int request, va_list ap);

ssize_t mp_uart_port_write(mp_device_id_t devid, void const * buf, size_t nbyte, mp_tick_t timeout);
ssize_t mp_uart_port_read(mp_device_id_t devid, void * buf, size_t nbyte, mp_tick_t timeout);

int mp_uart_port_waitEndTransmit(mp_device_id_t devid, mp_tick_t timeout);
int mp_uart_port_waitDataReceive(mp_device_id_t devid, mp_tick_t timeout);
//int mp_uart_port_waitFrameReceive(mp_device_id_t devid, mp_tick_t timeout);


// Static inline ISR -----------------------------------------------------------
__attribute__((always_inline))
static inline void mp_uart_port_usartx_fifo_isr(mp_uart_port_t * dev)
{
    USART_TypeDef * uartx = dev->uartx;
    
#ifdef MP_USE_FREERTOS
    MP_DEVICE(dev)->higherPriorityTaskWoken = pdFALSE;
#endif // MP_USE_FREERTOS
    
    // -- Manage transmit data --
    
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
        
        // Send synchro to unblock task
        MP_DEVICE_SEND_SYNC_FROM_ISR(dev, MP_UART_PORT_SYNC_TXFIFO_NO_FULL);
    }

    // The USART Transition is complete ?
    if (    LL_USART_IsEnabledIT_TC(uartx) &&
            LL_USART_IsActiveFlag_TC(uartx))
    {
        // This interruption is enable only to wakeup the CPU for the
        // waitEndTransmit() function. So, disable only it.
        LL_USART_DisableIT_TC(uartx);
        
        // Send synchro to unblock task
        MP_DEVICE_SEND_SYNC_FROM_ISR(dev, MP_UART_PORT_SYNC_TX_COMPLETE);
    }
    
    // -- Manage receive data --
    
    // The USART Rx FiFo is full ?
    if (    LL_USART_IsEnabledIT_RXFF(uartx)    &&
            LL_USART_IsActiveFlag_RXFF(uartx))
    {
        do
        {
            // Get and push in FiFo the new byte
            uint8_t byte = LL_USART_ReceiveData8(uartx);
            MP_FIFO_PUSH_BYTE(dev->fifoRx, byte);
            
            // Disable RX FIFO Full Interrupt if the data Rx FiFo is full.
            if (mp_fifo_isFull(dev->fifoRx))
            {
                LL_USART_DisableIT_RXFF(uartx);
                break;
            }
        }
        while(LL_USART_IsActiveFlag_RXNE_RXFNE(uartx));
    }
    
    // A byte has be received ?
    if (    LL_USART_IsEnabledIT_RXNE_RXFNE(uartx) &&
            LL_USART_IsActiveFlag_RXNE_RXFNE(uartx))
    {
        // This interruption is enable to wakeup the CPU for the
        // mp_uart_port_read() function.
        
        // Clean OverRun Error Flag
        LL_USART_ClearFlag_ORE(uartx);
        
        // We need to read a byte and push to the Rx FiFo for than Rx FiFo is no
        // empty and unblock mp_uart_port_read() function.
        uint8_t byte = LL_USART_ReceiveData8(uartx);
        MP_FIFO_PUSH_BYTE(dev->fifoRx, byte);
        
        // Disable 'RX Not Empty' Interrupt used for wakeup.
        LL_USART_DisableIT_RXNE_RXFNE(uartx);
        
        // Disable RX FIFO Full Interrupt if the data Rx FiFo is full.
        if (mp_fifo_isFull(dev->fifoRx))
            LL_USART_DisableIT_RXFF(uartx);
        
        // Send synchro to unblock task
        MP_DEVICE_SEND_SYNC_FROM_ISR(dev, MP_UART_PORT_SYNC_RXFIFO_NO_EMPTY);
    }
    
#ifdef MP_USE_FREERTOS
    portYIELD_FROM_ISR(MP_DEVICE(dev)->higherPriorityTaskWoken);
#endif // MP_USE_FREERTOS
}

__attribute__((always_inline))
static inline void mp_uart_port_usartx_isr(mp_uart_port_t * dev)
{
    USART_TypeDef * uartx = dev->uartx;
    
#ifdef MP_USE_FREERTOS
    MP_DEVICE(dev)->higherPriorityTaskWoken = pdFALSE;
#endif // MP_USE_FREERTOS
    
    // -- Manage transmit data --
    
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
            
        // Send synchro to unblock task
        MP_DEVICE_SEND_SYNC_FROM_ISR(dev, MP_UART_PORT_SYNC_TXFIFO_NO_FULL);
    }
    
    // The USART Transition is complete ?
    if (    LL_USART_IsEnabledIT_TC(uartx) &&
            LL_USART_IsActiveFlag_TC(uartx))
    {
        // This interruption is enable only to wakeup the CPU for the
        // waitEndTransmit() function. So, disable only it.
        LL_USART_DisableIT_TC(uartx);

        // Send synchro to unblock task
        MP_DEVICE_SEND_SYNC_FROM_ISR(dev, MP_UART_PORT_SYNC_TX_COMPLETE);
    }
    
    // -- Manage receive data --
    
    // The USART Rx is not empty ? 
    if (    LL_USART_IsEnabledIT_RXNE_RXFNE(uartx)    &&
            LL_USART_IsActiveFlag_RXNE_RXFNE(uartx))
    {
        // Clean OverRun Error Flag
        LL_USART_ClearFlag_ORE(uartx);
        
        // Get and push in FiFo the new byte
        uint8_t byte = LL_USART_ReceiveData8(uartx);
        MP_FIFO_PUSH_BYTE(dev->fifoRx, byte);
        
        // Disable 'RX Not Empty' Interrupt if the data Rx FiFo is full.
        if (mp_fifo_isFull(dev->fifoRx))
            LL_USART_DisableIT_RXNE_RXFNE(uartx);
            
        // Send synchro to unblock task
        MP_DEVICE_SEND_SYNC_FROM_ISR(dev, MP_UART_PORT_SYNC_RXFIFO_NO_EMPTY);
    }
    
#ifdef MP_USE_FREERTOS
    portYIELD_FROM_ISR(MP_DEVICE(dev)->higherPriorityTaskWoken);
#endif // MP_USE_FREERTOS
}

#ifdef USART1
    __attribute__((always_inline))
    static inline void mp_uart_port_usart1_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_usart1_dev;
        
        if (IS_UART_FIFO_INSTANCE(USART1))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // USART1

#ifdef USART2
    __attribute__((always_inline))
    static inline void mp_uart_port_usart2_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_usart2_dev;
        
        if (IS_UART_FIFO_INSTANCE(USART2))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // USART2

#ifdef USART3
    __attribute__((always_inline))
    static inline void mp_uart_port_usart3_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_usart3_dev;
        
        if (IS_UART_FIFO_INSTANCE(USART3))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // USART3

#ifdef USART4
    __attribute__((always_inline))
    static inline void mp_uart_port_usart4_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_usart4_dev;
        
        if (IS_UART_FIFO_INSTANCE(USART4))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // USART4

#ifdef USART5
    __attribute__((always_inline))
    static inline void mp_uart_port_usart5_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_usart5_dev;
        
        if (IS_UART_FIFO_INSTANCE(USART5))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // USART5

#ifdef USART6
    __attribute__((always_inline))
    static inline void mp_uart_port_usart6_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_usart6_dev;
        
        if (IS_UART_FIFO_INSTANCE(USART6))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // USART6

#ifdef LPUART1
    __attribute__((always_inline))
    static inline void mp_uart_port_lpuart1_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_lpuart1_dev;
        
        if (IS_UART_FIFO_INSTANCE(LPUART1))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // LPUART1

#ifdef LPUART2
    __attribute__((always_inline))
    static inline void mp_uart_port_lpuart2_isr()
    {
        mp_uart_port_t * dev = _mp_uart_port_lpuart2_dev;
        
        if (IS_UART_FIFO_INSTANCE(LPUART2))
            mp_uart_port_usartx_fifo_isr(dev);
        else
            mp_uart_port_usartx_isr(dev);
    }
#endif // LPUART2

#endif // MP_UART_PORT_H
