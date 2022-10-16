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

#if defined MP_USE_FREERTOS
    MP_DEVICE(dev)->taskToNotify = xTaskGetCurrentTaskHandle();
#endif
    
    // USART1
    // =================================
    #if defined(MP_USART1_RX_GPIO_Port) || defined(MP_USART1_TX_GPIO_Port)
    #ifndef USART1
    #error "MP_USART1_*** is defined in mpHardMap.h but the USART1 is not available on your stm32g0 !"
    #endif
    if (uartx == USART1)
    {
        uint32_t transferDirection = LL_LPUART_DIRECTION_NONE;
        
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
        uint32_t transferDirection = LL_LPUART_DIRECTION_NONE;
        
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
        
        #ifdef MP_USART2_RX_GPIO_Port
        // Enable RX FIFO Full Interrupt
        LL_USART_EnableIT_RXFF(USART2);
        #endif
        
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
                //(databit == MP_UART_DATA_9BITS), "Length data bits is not sported")
    
    //// Check stopbit compatibility
    //mp_assert(  (stopbit == MP_UART_STOPBIT_0_5) ||
                //(stopbit == MP_UART_STOPBIT_1)   ||
                //(stopbit == MP_UART_STOPBIT_1_5) ||
                //(stopbit == MP_UART_STOPBIT_2), "Stop Bit not sported")
    
    //// Check stopbit compatibility
    //mp_assert(  (databit != MP_UART_DATA_9BITS) ||
                //(databit == MP_UART_DATA_9BITS) &&
                //(parity == MP_UART_PARITY_NO), "Stop Bit not sported")
    
    
    
    USART_TypeDef * uartx = MP_PORT_UART_GET(devid)->uartx;
    
    
    
    uint32_t DataWidth;
    switch(databit)
    {
        case MP_UART_DATA_7BITS: DataWidth = LL_USART_DATAWIDTH_7B; break;
        case MP_UART_DATA_8BITS: DataWidth = LL_USART_DATAWIDTH_8B; break;
        //case MP_UART_DATA_9BITS: DataWidth = LL_USART_DATAWIDTH_9B; break;
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

// Ecrit dans la fifo Tx temp qu'il y a des donner dans le buffer (@p buf) et que le
// timeout n'est pas ecouler.
// Note: Le time out est recharger a chaque foi qu'un octer peut étre ecri dans la fifo Tx
ssize_t mp_uart_port_write(mp_device_id_t devid, const void * buf, size_t nbyte, mp_tick_t timeout)
{
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;
    size_t n = 0;
    
    // No byte to send ?
    if (nbyte == 0)
        return 0;
    
    // Mask of CR1 to disable and enable interruption to push data in the
    // Tx FiFo safely. The interruption used depend the uart mode, in Fifo mode
    // or not. 
    uint32_t cr1ItMask = 0;
    if (LL_USART_IsEnabledFIFO(uartx))
    {
        // In USART FiFo mode the 'TX FIFO Empty Interrupt' flag is used to
        // manage the transition and limit the number of interruption.
        // See: mp_uart_port_usartx_fifo_isr().
        cr1ItMask = USART_CR1_TXFEIE;
    }
    else
    {
        // In USART standard mode the 'TX Empty' flag is used to manage the
        // transition. See mp_uart_port_usartx_isr().
        cr1ItMask = USART_CR1_TXEIE_TXFNFIE;
    }
        
    // Write all data until complete or timeout.
    do
    {
        // Wait synchro to available byte in TX FiFo
        MP_DEVICE_WAIT_SYNC(dev,    !mp_fifo_isFull(dev->fifoTx),
                                    MP_UART_PORT_SYNC_TXFIFO_NO_FULL,
                                    timeout, return n);
        
        // Push data to FiFo
        ATOMIC_CLEAR_BIT(uartx->CR1, cr1ItMask); 
        n += mp_fifo_push(dev->fifoTx, buf+n, nbyte-n);
        ATOMIC_SET_BIT(uartx->CR1, cr1ItMask);
        
    }while (n < nbyte);

    return n;
}

// Si il n'y pas pas de donnée de disponible attem au maximume un timout.
// Li les donner de la fifo de nbyte ou moin. si aucune donnée est displonible 
// a laplle de la fonction atten au maximume un timeout.
// Note: Le time out commense a lapelle de la fonction
ssize_t mp_uart_port_read(mp_device_id_t devid, void * buf, size_t nbyte, mp_tick_t timeout)
{
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;
    size_t n = 0;
    
    // No byte to receive ?
    if (nbyte == 0)
        return 0;

    // Wait available byte or timeout
    if (mp_uart_port_waitDataReceive(devid, timeout) <= 0)
        return 0;

    // Mask of CR1 to disable and enable interruption to pop data in the
    // Rx FiFo safely. The interruption used depend the uart mode, in Fifo mode
    // or not. 
    uint32_t cr1ItMask = 0;
    if (LL_USART_IsEnabledFIFO(uartx))
    {
        // In USART FiFo mode the 'RX FIFO Full Interrupt' flag is used to
        // manage the reception and limit the number of interruption.
        // See: mp_uart_port_usartx_fifo_isr().
        cr1ItMask = USART_CR1_RXFFIE;
    }
    else
    {
        // In USART standard mode the 'RX Not Empty' flag is used to manage the
        // reception. See mp_uart_port_usartx_isr().
        cr1ItMask = USART_CR1_RXNEIE_RXFNEIE;
    }

    // Disable 'RX FIFO Full Interrupt' or 'RX Not Empty'
    ATOMIC_CLEAR_BIT(uartx->CR1, cr1ItMask); 
    
    // Tack bytes from Rx FiFo
    n += mp_fifo_pop(dev->fifoRx, buf+n, nbyte-n);
    
    // Tack additional bytes from USART Rx
    while(  (n < nbyte) &&
            LL_USART_IsActiveFlag_RXNE_RXFNE(uartx))
    {
        uint8_t byte = LL_USART_ReceiveData8(uartx);
        *((uint8_t*)buf + n) = byte;
        n += 1;
    }
    
    // Enable 'RX FIFO Full Interrupt' or 'RX Not Empty'
    ATOMIC_SET_BIT(uartx->CR1, cr1ItMask);
    
    return n;
}


int mp_uart_port_waitEndTransmit(mp_device_id_t devid, mp_tick_t timeout)
{
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;

    LL_USART_EnableIT_TC(uartx);
    MP_DEVICE_WAIT_SYNC(dev,    LL_USART_IsActiveFlag_TC(uartx),
                                MP_UART_PORT_SYNC_TX_COMPLETE,
                                timeout, return 0);
    return 1;
}

int mp_uart_port_waitDataReceive(mp_device_id_t devid, mp_tick_t timeout)
{
    mp_uart_port_t * dev = MP_PORT_UART_GET(devid);
    USART_TypeDef * uartx = dev->uartx;
    
    LL_USART_EnableIT_RXNE_RXFNE(uartx);
    MP_DEVICE_WAIT_SYNC(dev,    !mp_fifo_isEmpty(dev->fifoRx),
                                MP_UART_PORT_SYNC_RXFIFO_NO_EMPTY,
                                timeout, return 0);
    return mp_fifo_used(dev->fifoRx);
}

int mp_uart_port_ctl(mp_device_id_t devid, int request, va_list ap)
{
    (void)devid;
    (void)request;
    (void)ap;
    
    
    int ret = 0;
    switch(request)
    {
        // Todo: mp_assert() ou mp_err plustot que de retrouner -1
        default: ret = -1; break;
        //default: mp_err("The request=%u to control uart is not sported", request); break;
    }
    
    return ret;
}
