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
#include "mp.h"

// Protected global variables --------------------------------------------------

// Implemented functions -------------------------------------------------------
int mp_interrupt_port_init()
{
    #define MP_INT_ISR(irq, priority, isr_imp)                         \
        NVIC_SetPriority(irq##_IRQn, priority);                        \
        NVIC_EnableIRQ(irq##_IRQn);
        
    MP_INTERRUPT_TABLE
    #undef MP_INT_ISR

    return 0;
}



#define MP_INT_ISR(irq, priority, isr_imp)                             \
    void irq##_IRQHandler()                                            \
    {                                                                  \
        isr_imp;                                                       \
    }
    
MP_INTERRUPT_TABLE
#undef MP_INT_ISR

