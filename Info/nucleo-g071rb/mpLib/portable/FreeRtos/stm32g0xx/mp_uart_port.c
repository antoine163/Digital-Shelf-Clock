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
#include "mpHardMap.h"

// St Low level
#include <stm32g0xx_ll_rcc.h>
#include <stm32g0xx_ll_bus.h>
#include <stm32g0xx_ll_gpio.h>

// Std
#include <string.h>

// Protected global variables --------------------------------------------------
mp_port_uart_t *_mp_stm_usart1_drv = NULL;
mp_port_uart_t *_mp_stm_usart2_drv = NULL;
mp_port_uart_t *_mp_stm_usart3_drv = NULL;
mp_port_uart_t *_mp_stm_usart4_drv = NULL;
mp_port_uart_t *_mp_stm_usart5_drv = NULL;
mp_port_uart_t *_mp_stm_usart6_drv = NULL;
mp_port_uart_t *_mp_stm_lpuart1_drv = NULL;
mp_port_uart_t *_mp_stm_lpuart2_drv = NULL;

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
 * 
 * USART1_IRQHandler
 * USART2_LPUART2_IRQHandler
 * USART3_4_5_6_LPUART1_IRQHandler
 * 
 * 
 * 
 * - USART1_IRQHandler
 * - USART2_IRQHandler
 * - USART3_4_LPUART1_IRQHandler
 * 
 */

/**
 * @ingroup port_stm32g0x
 * @brief Initialize a Uart peripteral
 * 
 * This function initialize the GPIO pins and the UART peripheral clock.
 * 
 * @warning The peripheral clock of GPIO Port used here (ex: Tx and Rx pin) must
 * be enable before call this function.
 * 
 * @warning The IRQ used here (ex: USART2_IRQn) must be enable...
 */
int mp_port_uart_init(mp_port_uart_t *drv,  USART_TypeDef *dev)
{
    memset(drv, 0, sizeof(mp_port_uart_t));
    drv->dev = dev;
    
    // Init GPIO of USART2
    #if defined(MP_USART2_RX_GPIO_Port) || defined(MP_USART2_TX_GPIO_Port)
    if (dev == USART2)
    {
        //#ifdef MP_USART2_RX_GPIO_Port
        //mp_gpio_config(drv_gpio, MP_USART2_RX)
        //#endif
        
        //#ifdef MP_USART2_TX_GPIO_Port
        //#endif
        
        #ifdef MP_USART2_RX_GPIO_Port
            // Configure RX Pin as : Alternate function, High Speed, PushPull, Pull up.
            LL_GPIO_SetPinMode(         MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, LL_GPIO_MODE_ALTERNATE);
            if(MP_USART2_RX_Pin <= LL_GPIO_PIN_7)
                LL_GPIO_SetAFPin_0_7(   MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_AF);
            else
                LL_GPIO_SetAFPin_8_15(  MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_AF);
            LL_GPIO_SetPinSpeed(        MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_SPEED);
            LL_GPIO_SetPinPull(         MP_USART2_RX_GPIO_Port, MP_USART2_RX_Pin, MP_USART2_RX_PULL);
        #endif
        
        #ifdef MP_USART2_TX_GPIO_Port
            // Configure TX Pin as : Alternate function, High Speed, PushPull, Pull up.
            LL_GPIO_SetPinMode(         MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, LL_GPIO_MODE_ALTERNATE);
            if(MP_USART2_TX_Pin <= LL_GPIO_PIN_7)
                LL_GPIO_SetAFPin_0_7(   MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_AF);
            else
                LL_GPIO_SetAFPin_8_15(  MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_AF);
            LL_GPIO_SetPinSpeed(        MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_SPEED);
            LL_GPIO_SetPinOutputType(   MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_OUTPUT);
            LL_GPIO_SetPinPull(         MP_USART2_TX_GPIO_Port, MP_USART2_TX_Pin, MP_USART2_TX_PULL);
        #endif

        // Enable IRQ
        //NVIC_SetPriority(USART2_IRQn, MP_USART2_IRQ_PRIORITY);
        //NVIC_SetPriority(USART2_IRQn, MP_USART2_IRQ_PRIORITY);
        NVIC_EnableIRQ(USART2_IRQn);
        
        //mp_irq_enable(drv_irq, USART2);
        
        // Enable the USART2 peripheral clock and clock source.
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
        
        // Set USART2 clock source as PCLK1
        LL_RCC_SetUSARTClockSource(MP_USART2_CLKSOURCE);
        
        // Set Usart2 pointer.
        _mp_stm_usart2_drv = drv;
        
        #ifdef MP_USART2_RX_GPIO_Port
            // Enable RXNE and Error interrupts.
            LL_USART_EnableIT_RXNE_RXFNE(USART2);
        #endif
        
        return 0;
    }
    #endif
    
    
    return -1;
}

int mp_port_uart_deinit(mp_port_uart_t *drv)
{
    (void)drv;
    return -1;
}

int mp_port_uart_config(mp_port_uart_t *drv,
                        mp_uart_baudrate_t baudrate,
                        mp_uart_databits_t databit,
                        mp_uart_parity_t parity,
                        mp_uart_stopbit_t stopbit)
{  
    (void)baudrate;
    (void)databit;
    (void)parity;
    (void)stopbit;
    
    /* (4) Configure USART functional parameters ********************************/
    
    /* Disable USART prior modifying configuration registers */
    /* Note: Commented as corresponding to Reset value */
    // LL_USART_Disable(drv->dev);
    
    /* TX/RX direction */
    LL_USART_SetTransferDirection(drv->dev, LL_USART_DIRECTION_TX_RX);
    
    /* 8 data bit, 1 start bit, 1 stop bit, no parity */
    LL_USART_ConfigCharacter(drv->dev, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
    
    /* No Hardware Flow control */
    /* Reset value is LL_USART_HWCONTROL_NONE */
    // LL_USART_SetHWFlowCtrl(drv->dev, LL_USART_HWCONTROL_NONE);
    
    /* Oversampling by 16 */
    /* Reset value is LL_USART_OVERSAMPLING_16 */
    // LL_USART_SetOverSampling(drv->dev, LL_USART_OVERSAMPLING_16);
    
    /* Set Baudrate to 115200 using APB frequency set to 56000000 Hz */
    /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
    /* Ex :
        Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
    
        In this example, Peripheral Clock is expected to be equal to 56000000 Hz => equal to SystemCoreClock
    */
    LL_USART_SetBaudRate(drv->dev, SystemCoreClock, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16, (uint32_t)baudrate); 
    
    /* (5) Enable USART *********************************************************/
    LL_USART_Enable(drv->dev);
    
    /* Polling USART initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(drv->dev))) || (!(LL_USART_IsActiveFlag_REACK(drv->dev))))
    { 
    }
    
    return 0;
}

int mp_port_uart_write(mp_port_uart_t *drv, const void *buf, size_t nbyte)
{
    (void)buf;
    (void)nbyte;
    
    memcpy(drv->txBuf, buf, nbyte);
    drv->lenSend = nbyte;
    drv->iSend = 1;
    
    /* Start USART transmission : Will initiate TXE interrupt after TDR register is empty */
    LL_USART_TransmitData8(drv->dev, drv->txBuf[0]); 

    /* Enable TXE interrupt */
    LL_USART_EnableIT_TXE(drv->dev); 
    
    return 0;
}

int mp_port_uart_read(mp_port_uart_t *drv, void *buf, size_t nbyte)
{
    (void)drv;
    (void)buf;
    (void)nbyte;
    
    return 0;
}

int mp_port_uart_ctl(mp_port_uart_t *drv, int request, va_list ap)
{
    (void)drv;
    (void)request;
    (void)ap;
    
    return 0;
}
