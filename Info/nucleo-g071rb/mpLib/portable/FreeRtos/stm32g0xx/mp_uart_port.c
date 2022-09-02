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
#include "mp/drivers/uart.h"
#include "mpHardMap.h"

// St Low level
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>

// Std
#include <string.h>

// Protected global variables --------------------------------------------------
mp_uart_port_t * _mp_uart_port_usart1_dev = NULL;
mp_uart_port_t * _mp_uart_port_usart2_dev = NULL;
mp_uart_port_t * _mp_uart_port_usart3_dev = NULL;
mp_uart_port_t * _mp_uart_port_usart4_dev = NULL;
mp_uart_port_t * _mp_uart_port_usart5_dev = NULL;
mp_uart_port_t * _mp_uart_port_usart6_dev = NULL;
mp_uart_port_t * _mp_uart_port_lpuart1_dev = NULL;
mp_uart_port_t * _mp_uart_port_lpuart2_dev = NULL;

// Implemented functions -------------------------------------------------------

/**
 * 
 * Define of USART2 in mpHardMap.h file
 * 
 * - MP_USART2_TX_GPIO_Port
 * - MP_USART2_TX_Pin
 * - MP_USART2_TX_AF
 * - MP_USART2_TX_SPEED
 * - MP_USART2_TX_PULL
 * - MP_USART2_TX_OUTPUT
 * - MP_USART2_TX_PIN_LEVEL
 * - 
 * - MP_USART2_RX_GPIO_Port
 * - MP_USART2_RX_Pin
 * - MP_USART2_RX_AF
 * - MP_USART2_RX_SPEED
 * - MP_USART2_RX_PULL
 * - MP_USART2_RX_OUTPUT
 * - MP_USART2_RX_PIN_LEVEL
 *
 * - MP_USART2_CLKSOURCE
 * - MP_USART2_TXRX_SWAPPED
 * 
 * Interrupt:
 * - MP_USART1_IRQ_PRIORITY
 * - MP_USART2_IRQ_PRIORITY
 * - MP_USART3_IRQ_PRIORITY
 * - MP_USART4_IRQ_PRIORITY
 * - MP_USART5_IRQ_PRIORITY
 * - MP_USART6_IRQ_PRIORITY
 * 
 * 
 * IRQHandler :
 * - USART1
 * - USART2_LPUART2
 * - USART3_4_5_6_LPUART1
 * - USART2
 * - USART3_4_LPUART1
 * 
 */
 
 // If the USART is compatible with FiFo mode this will be enable.
    // This allows to reduce the number of interruption to receive/transfer 
    // the data.

/**
 * @ingroup port_stm32g0x
 * @brief Initialize a Uart peripteral
 * 
 * This function initialize the GPIO pins and the UART peripheral clock.
 * 
 * @warning The peripheral clock of GPIO Port used here (ex: Tx and Rx pin) must
 * be enable before call this function.
 * 
 * @todo a revoire
 */
int mp_uart_port_init(mp_device_id_t devid)
{
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;
    uint32_t transferDirection;
    
    // USART1
    // =================================
    #if defined(MP_USART1_RX_GPIO_Port) || defined(MP_USART1_TX_GPIO_Port)
    #ifndef USART1
    #error "MP_USART1_*** is defined in mpHardMap.h but the USART1 is not available on your stm32g0 !"
    #endif
    if (uartx == USART1)
    {
        transferDirection = LL_LPUART_DIRECTION_NONE;
        
        #ifdef MP_USART1_RX_GPIO_Port
            transferDirection |= LL_LPUART_DIRECTION_RX;
        
            // Configure RX Pin as : Alternate function
            LL_GPIO_SetPinMode(         MP_USART1_RX_GPIO_Port, MP_USART1_RX_Pin, LL_GPIO_MODE_ALTERNATE);
            if(MP_USART1_RX_Pin <= LL_GPIO_PIN_7)
                LL_GPIO_SetAFPin_0_7(   MP_USART1_RX_GPIO_Port, MP_USART1_RX_Pin, MP_USART1_RX_AF);
            else
                LL_GPIO_SetAFPin_8_15(  MP_USART1_RX_GPIO_Port, MP_USART1_RX_Pin, MP_USART1_RX_AF);
            LL_GPIO_SetPinSpeed(        MP_USART1_RX_GPIO_Port, MP_USART1_RX_Pin, MP_USART1_RX_SPEED);
            LL_GPIO_SetPinPull(         MP_USART1_RX_GPIO_Port, MP_USART1_RX_Pin, MP_USART1_RX_PULL);
            
            // Init Rx FiFo
            static mp_fifo_t uart1FifoRx;
            static uint8_t uart1FifoRxStorageBuffer[MP_USART1_RX_FIFO_SIZE];
            mp_fifo_init(&uart1FifoRx, uart1FifoRxStorageBuffer, MP_USART1_RX_FIFO_SIZE);
            dev->fifoRx = &uart1FifoRx;
        #endif
        
        #ifdef MP_USART1_TX_GPIO_Port
            transferDirection |= LL_LPUART_DIRECTION_TX;
            
            // Configure TX Pin as : Alternate function
            LL_GPIO_SetPinMode(         MP_USART1_TX_GPIO_Port, MP_USART1_TX_Pin, LL_GPIO_MODE_ALTERNATE);
            if(MP_USART1_TX_Pin <= LL_GPIO_PIN_7)
                LL_GPIO_SetAFPin_0_7(   MP_USART1_TX_GPIO_Port, MP_USART1_TX_Pin, MP_USART1_TX_AF);
            else
                LL_GPIO_SetAFPin_8_15(  MP_USART1_TX_GPIO_Port, MP_USART1_TX_Pin, MP_USART1_TX_AF);
            LL_GPIO_SetPinSpeed(        MP_USART1_TX_GPIO_Port, MP_USART1_TX_Pin, MP_USART1_TX_SPEED);
            LL_GPIO_SetPinOutputType(   MP_USART1_TX_GPIO_Port, MP_USART1_TX_Pin, MP_USART1_TX_OUTPUT);
            LL_GPIO_SetPinPull(         MP_USART1_TX_GPIO_Port, MP_USART1_TX_Pin, MP_USART1_TX_PULL);
            
            // Init Tx FiFo
            static mp_fifo_t uart1FifoTx;
            static uint8_t uart1FifoTxStorageBuffer[MP_USART1_TX_FIFO_SIZE];
            mp_fifo_init(&uart1FifoTx, uart1FifoTxStorageBuffer, MP_USART1_TX_FIFO_SIZE);
            dev->fifoTx = &uart1FifoTx;
        #endif
        
        // Enable the USART1 peripheral clock and clock source.
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
        
        // Set USART1 clock source as PCLK1
        LL_RCC_SetUSARTClockSource(MP_USART1_CLKSOURCE);
        
        // Save pointer of usart device.
        _mp_uart_port_usart1_dev = dev;
        
        // Set TX/RX direction
        LL_LPUART_SetTransferDirection(USART1, transferDirection);
        
        #ifdef MP_USART1_RX_PIN_LEVEL
        LL_USART_SetRXPinLevel(USART1, MP_USART1_RX_PIN_LEVEL);
        #endif
        
        #ifdef MP_USART1_TX_PIN_LEVEL
        LL_USART_SetTXPinLevel(USART1, MP_USART1_TX_PIN_LEVEL);
        #endif
        
        //#ifdef MP_USART1_RX_GPIO_Port
        //// Enable RXNE and Error interrupts.
        //LL_USART_EnableIT_RXNE_RXFNE(USART1);
        //#endif
        
        return 0;
    }
    #endif
    
    // USART2
    // =================================
    #if defined(MP_USART2_RX_GPIO_Port) || defined(MP_USART2_TX_GPIO_Port)
    #ifndef USART2
    #error "MP_USART2_*** is defined in mpHardMap.h but the USART2 is not available on your stm32g0 !"
    #endif
    if (uartx == USART2)
    {
        transferDirection = LL_LPUART_DIRECTION_NONE;
        
        #ifdef MP_USART2_RX_GPIO_Port
            transferDirection |= LL_LPUART_DIRECTION_RX;
            
            // Configure RX Pin as : Alternate function
            LL_GPIO_SetPinMode(         MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, LL_GPIO_MODE_ALTERNATE);
            if(MP_USART2_RX_Pin <= LL_GPIO_PIN_7)
                LL_GPIO_SetAFPin_0_7(   MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_AF);
            else
                LL_GPIO_SetAFPin_8_15(  MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_AF);
            LL_GPIO_SetPinSpeed(        MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_SPEED);
            LL_GPIO_SetPinPull(         MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_PULL);
            
            // Init Rx FiFo
            static mp_fifo_t uart2FifoRx;
            static uint8_t uart2FifoRxStorageBuffer[MP_USART2_RX_FIFO_SIZE];
            mp_fifo_init(&uart2FifoRx, uart2FifoRxStorageBuffer, MP_USART2_RX_FIFO_SIZE);
            dev->fifoRx = &uart2FifoRx;
        #endif
        
        #ifdef MP_USART2_TX_GPIO_Port
            transferDirection |= LL_LPUART_DIRECTION_TX;
            
            // Configure TX Pin as : Alternate function
            LL_GPIO_SetPinMode(         MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, LL_GPIO_MODE_ALTERNATE);
            if(MP_USART2_TX_Pin <= LL_GPIO_PIN_7)
                LL_GPIO_SetAFPin_0_7(   MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_AF);
            else
                LL_GPIO_SetAFPin_8_15(  MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_AF);
            LL_GPIO_SetPinSpeed(        MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_SPEED);
            LL_GPIO_SetPinOutputType(   MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_OUTPUT);
            LL_GPIO_SetPinPull(         MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_PULL);
            
            // Init Tx FiFo
            static mp_fifo_t uart2FifoTx;
            static uint8_t uart2FifoTxStorageBuffer[MP_USART2_TX_FIFO_SIZE];
            mp_fifo_init(&uart2FifoTx, uart2FifoTxStorageBuffer, MP_USART2_TX_FIFO_SIZE);
            dev->fifoTx = &uart2FifoTx;
        #endif
        
        // Enable the USART2 peripheral clock and clock source.
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
        
        // Set USART2 clock source as PCLK1
        LL_RCC_SetUSARTClockSource(MP_USART2_CLKSOURCE);
        
        // Save pointer of usart device.
        _mp_uart_port_usart2_dev = dev;
        
        // Set TX/RX direction
        LL_LPUART_SetTransferDirection(USART2, transferDirection);
        
        #ifdef MP_USART2_RX_PIN_LEVEL
        LL_USART_SetRXPinLevel(USART2, MP_USART2_RX_PIN_LEVEL);
        #endif
        
        #ifdef MP_USART2_TX_PIN_LEVEL
        LL_USART_SetTXPinLevel(USART2, MP_USART2_TX_PIN_LEVEL);
        #endif
        
        //#ifdef MP_USART2_RX_GPIO_Port
        //// Enable RXNE and Error interrupts.
        //LL_USART_EnableIT_RXNE_RXFNE(USART2);
        //#endif
        
        return 0;
    }
    #endif
    
    return -1;
}

int mp_uart_port_deinit(mp_device_id_t devid)
{
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;
    
    // Disable RXNE and Error interrupts.
    LL_USART_DisableIT_RXNE_RXFNE(uartx);
    
    // Disable usart
    LL_LPUART_Disable(uartx);
    
    // USART1
    // =================================
    #if defined(USART1_RX_GPIO_Port) || defined(USART1_TX_GPIO_Port)
    if(uartx == USART1)
    {
        // Remove pointer of usart device.
        _mp_uart_port_usart1_dev = NULL;
        
        // Disable the USART1 peripheral clock and clock source.
        LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_USART1);
        
        #ifdef USART1_RX_GPIO_Port
            // Configure RX Pin as : Analog input.
            LL_GPIO_SetPinMode(     USART1_RX_GPIO_Port, USART1_RX_Pin, LL_GPIO_MODE_ANALOG);
            LL_GPIO_SetPinSpeed(    USART1_RX_GPIO_Port, USART1_RX_Pin, LL_GPIO_SPEED_FREQ_MEDIUM);
            LL_GPIO_SetPinPull(     USART1_RX_GPIO_Port, USART1_RX_Pin, LL_GPIO_PULL_NO);
        #endif
        
        #ifdef USART1_TX_GPIO_Port
            // Configure TX Pin as : Analog input.
            LL_GPIO_SetPinMode(     USART1_TX_GPIO_Port, USART1_TX_Pin, LL_GPIO_MODE_ANALOG);
            LL_GPIO_SetPinSpeed(    USART1_TX_GPIO_Port, USART1_TX_Pin, LL_GPIO_SPEED_FREQ_MEDIUM);
            LL_GPIO_SetPinPull(     USART1_TX_GPIO_Port, USART1_TX_Pin, LL_GPIO_PULL_NO);
        #endif
        
        return 0;
    }
    #endif
    
    // USART2
    // =================================
    #if defined(USART2_RX_GPIO_Port) || defined(USART2_TX_GPIO_Port)
    if(uartx == USART2)
    {
        // Remove pointer of usart device.
        _mp_uart_port_usart2_dev = NULL;
        
        // Disable the USART2 peripheral clock and clock source.
        LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);
        
        #ifdef USART2_RX_GPIO_Port
            // Configure RX Pin as : Analog input.
            LL_GPIO_SetPinMode(     USART2_RX_GPIO_Port, USART2_RX_Pin, LL_GPIO_MODE_ANALOG);
            LL_GPIO_SetPinSpeed(    USART2_RX_GPIO_Port, USART2_RX_Pin, LL_GPIO_SPEED_FREQ_MEDIUM);
            LL_GPIO_SetPinPull(     USART2_RX_GPIO_Port, USART2_RX_Pin, LL_GPIO_PULL_NO);
        #endif
        
        #ifdef USART2_TX_GPIO_Port
            // Configure TX Pin as : Analog input.
            LL_GPIO_SetPinMode(     USART2_TX_GPIO_Port, USART2_TX_Pin, LL_GPIO_MODE_ANALOG);
            LL_GPIO_SetPinSpeed(    USART2_TX_GPIO_Port, USART2_TX_Pin, LL_GPIO_SPEED_FREQ_MEDIUM);
            LL_GPIO_SetPinPull(     USART2_TX_GPIO_Port, USART2_TX_Pin, LL_GPIO_PULL_NO);
        #endif
        
        return 0;
    }
    #endif
    
    return -1;
}

// Note: pas de pariter en databit=9
// Note: la vertion 9bit non suporter par ce driver mais pouret étre implmenter ...
int mp_uart_port_config(mp_device_id_t devid, mp_uart_baudrate_t baudrate,
                        mp_uart_databits_t databit, mp_uart_parity_t parity,
                        mp_uart_stopbit_t stopbit)
{
    // Todo: fair des asserts
    //// Check databit compatibility
    //mp_assert(  (databit == MP_UART_DATA_7BITS) ||
                //(databit == MP_UART_DATA_8BITS) ||
                //(databit == MP_UART_DATA_9BITS) )
    
    //// Check stopbit compatibility
    //mp_assert(  (stopbit == MP_UART_STOPBIT_0_5) ||
                //(stopbit == MP_UART_STOPBIT_1)   ||
                //(stopbit == MP_UART_STOPBIT_1_5) ||
                //(stopbit == MP_UART_STOPBIT_2) )
    
    //// Check stopbit compatibility
    //mp_assert(  (databit != MP_UART_DATA_9BITS) ||
                //(databit == MP_UART_DATA_9BITS) &&
                //(parity == MP_UART_PARITY_NO) )
    
    
    
    USART_TypeDef * uartx = MP_PORT_UART_GET(devid)->uartx;
    
    
    
    uint32_t DataWidth;
    switch(databit)
    {
        case MP_UART_DATA_7BITS: DataWidth = LL_USART_DATAWIDTH_7B; break;
        case MP_UART_DATA_8BITS: DataWidth = LL_USART_DATAWIDTH_8B; break;
        case MP_UART_DATA_9BITS: DataWidth = LL_USART_DATAWIDTH_9B; break;
        default: return -1;
    }
    
    uint32_t Parity;
    switch(parity)
    {
        case MP_UART_PARITY_NO:   Parity = LL_USART_PARITY_NONE; break;
        case MP_UART_PARITY_ODD:  Parity = LL_USART_PARITY_ODD;  break;
        case MP_UART_PARITY_EVEN: Parity = LL_USART_PARITY_EVEN; break;
        default: return -1;
    }
    
    uint32_t StopBits;
    switch(stopbit)
    {
        case MP_UART_STOPBIT_0_5: StopBits = LL_USART_STOPBITS_0_5; break;
        case MP_UART_STOPBIT_1:   StopBits = LL_USART_STOPBITS_1;   break;
        case MP_UART_STOPBIT_1_5: StopBits = LL_USART_STOPBITS_1_5; break;
        case MP_UART_STOPBIT_2:   StopBits = LL_USART_STOPBITS_2;   break;
        default: return -1;
    }
    
    /* (4) Configure USART functional parameters ********************************/
    
    /* Disable USART prior modifying configuration registers */
    /* Note: Commented as corresponding to Reset value */
     LL_USART_Disable(uartx);
    
    /* TX/RX direction */
    //LL_USART_SetTransferDirection(uartx, LL_USART_DIRECTION_TX_RX);
    //LL_USART_SetTransferDirection(uartx, LL_USART_DIRECTION_TX);
    
    /* 8 data bit, 1 start bit, 1 stop bit, no parity */
    LL_USART_ConfigCharacter(uartx, DataWidth, Parity, StopBits);
    
    /* Oversampling by 16 */
    /* Reset value is LL_USART_OVERSAMPLING_16 */
    // LL_USART_SetOverSampling(uartx, LL_USART_OVERSAMPLING_16);
     LL_USART_SetOverSampling(uartx, LL_USART_OVERSAMPLING_8);
    
    /* Set Baudrate to 115200 using APB frequency set to 56000000 Hz */
    /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
    /* Ex :
        Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
    
        In this example, Peripheral Clock is expected to be equal to 56000000 Hz => equal to SystemCoreClock
    */
    LL_USART_SetBaudRate(uartx, SystemCoreClock, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_8, (uint32_t)baudrate); 
    //LL_USART_SetBaudRate(uartx, SystemCoreClock, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, (uint32_t)baudrate); 
    // Todo activer ou pas LL_USART_OVERSAMPLING_16
    
    // Enable a FiFo mode is the USART is compatible.
    if (IS_UART_FIFO_INSTANCE(uartx))
        LL_USART_EnableFIFO(uartx);
    
    /* (5) Enable USART *********************************************************/
    LL_USART_Enable(uartx);
    
    /* Polling USART initialisation */
    //while ((!(LL_USART_IsActiveFlag_TEACK(uartx))) || (!(LL_USART_IsActiveFlag_REACK(uartx))))
    //while ( !LL_USART_IsActiveFlag_TEACK(uartx) );
    
    return 0;
}

int mp_uart_port_write(mp_device_id_t devid, const void * buf, size_t nbyte)
{
    if (nbyte == 0)
        return 0;
        
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;
    int n = 0;
    
    // USART in FiFo mode ?
    if (LL_USART_IsEnabledFIFO(uartx)) // Yes
    {
        // Disable TXFE interrupt to to avoid conflict with the Tx FiFo
        LL_USART_DisableIT_TXFE(uartx);
        
        // If the FiFo is empty we can priming the transfer.
        bool startTx = mp_fifo_isEmpty(dev->fifoTx);
            
        n = mp_fifo_push(dev->fifoTx, buf, nbyte);
        
        if (startTx && LL_USART_IsActiveFlag_TXE_TXFNF(uartx))
        {
            // While USART FiFo is not full we can fill TDR.
            do
            {
                uint8_t byte;
                MP_FIFO_POP_BYTE(dev->fifoTx, &byte);
                LL_USART_TransmitData8(uartx, byte);
            }
            while(  LL_USART_IsActiveFlag_TXE_TXFNF(uartx) &&
                    !mp_fifo_isEmpty(dev->fifoTx));
                
        }
        
        if (!mp_fifo_isEmpty(dev->fifoTx))
            LL_USART_EnableIT_TXFE(uartx);
    }
    else // No
    {
        // Disable TXE interrupt to to avoid conflict with the Tx FiFo
        LL_USART_DisableIT_TXE_TXFNF(uartx);
        
        // If the FiFo is empty we can priming the transfer.
        bool txPriming = mp_fifo_isEmpty(dev->fifoTx);
        
        n = mp_fifo_push(dev->fifoTx, buf, nbyte);
        
        if (txPriming && LL_USART_IsActiveFlag_TXE_TXFNF(uartx))
        {
            uint8_t byte;
            MP_FIFO_POP_BYTE(dev->fifoTx, &byte);
            LL_USART_TransmitData8(uartx, byte);
        }
        
        if (!mp_fifo_isEmpty(dev->fifoTx))
            LL_USART_EnableIT_TXE_TXFNF(uartx);
    }
    
    return n;
}

int mp_uart_port_read(mp_device_id_t devid, void *buf, size_t nbyte)
{
    (void)devid;
    (void)buf;
    (void)nbyte;
    
    return 0;
}

int mp_uart_port_ctl(mp_device_id_t devid, int request, va_list ap)
{
    (void)devid;
    (void)request;
    (void)ap;
    
    return 0;
}
