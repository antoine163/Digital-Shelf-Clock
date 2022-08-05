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

#ifndef MP_FREERTOS_STM32G0XX_IRQ_H
#define MP_FREERTOS_STM32G0XX_IRQ_H

// Include ---------------------------------------------------------------------
#include "stm32g0xx.h"
// Don't include "mp/drivers/irq.h" here. It is "mp/drivers/irq.h" which
// include "mp_port_irq.h" after to have declare enum, struct, typedef, ...

// Structure -------------------------------------------------------------------
typedef struct
{
    IRQn_Type irq;
}mp_port_irq_t;

// Prototype functions ---------------------------------------------------------
static inline int mp_port_irq_init(mp_port_irq_t *drv, IRQn_Type irq)
{
    drv->irq = irq;
}

static inline int mp_port_irq_enable(mp_port_irq_t *drv, void *dev)
{
    if(0){}
    else if ( dev == USART3 )
    {
        drv->usart3_4_lpuart1 |= MP_IRQ_USART3_MASK;
        NVIC_EnableIRQ(USART3_4_LPUART1_IRQn);
    }
    else if ( dev == USART4 )
    {
        drv->usart3_4_lpuart1 |= MP_IRQ_USART4_MASK;
        NVIC_EnableIRQ(USART3_4_LPUART1_IRQn);
    }
    else if ( dev == LPUART1 )
    {
        drv->usart3_4_lpuart1 |= MP_IRQ_LPUART1_MASK;
        NVIC_EnableIRQ(USART3_4_LPUART1_IRQn);
    }
}

static inline int mp_port_irq_disable(mp_port_irq_t *drv, void *dev)
{
    if(0){}
    else if ( dev == USART3 )
    {
        drv->usart3_4_lpuart1 &= ~MP_IRQ_USART3_MASK;
        
        if (usart3_4_lpuart1 == 0)
            NVIC_DisableIRQ(USART3_4_LPUART1_IRQn);
    }
    else if ( dev == USART4 )
    {
        drv->usart3_4_lpuart1 &= ~MP_IRQ_USART4_MASK;
        
        if (usart3_4_lpuart1 == 0)
            NVIC_DisableIRQ(USART3_4_LPUART1_IRQn);
    }
    else if ( dev == LPUART1 )
    {
        drv->usart3_4_lpuart1 &= ~MP_IRQ_LPUART1_MASK;
        
        if (usart3_4_lpuart1 == 0)
            NVIC_DisableIRQ(USART3_4_LPUART1_IRQn);
    }
}

#endif // MP_FREERTOS_STM32G0XX_IRQ_H
