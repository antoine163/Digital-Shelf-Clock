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

#ifndef MP_FREERTOS_STM32G0XX_EXTI_H
#define MP_FREERTOS_STM32G0XX_EXTI_H

// Include ---------------------------------------------------------------------
#include "stm32g0xx.h"
// Don't include "mp/drivers/irq.h" here. It is "mp/drivers/irq.h" which
// include "mp_port_irq.h" after to have declare enum, struct, typedef, ...

// Structure -------------------------------------------------------------------


// Prototype functions ---------------------------------------------------------


static inline void mp_port_gpio_handler(uint32_t exti_line)
{
    mp_port_exti_t *drv = NULL;
    switch (exti_line)
    {
        case LL_EXTI_LINE_7: drv = _mp_stm_exti17_drv; break;
        case LL_EXTI_LINE_13: drv = _mp_stm_exti13_drv; break;
        default: assert() break;
    }
    
    
    if ((LL_EXTI_IsEnabledFallingTrig_0_31(exti_line) != RESET) &&
        (LL_EXTI_IsActiveFallingFlag_0_31(exti_line) != RESET))
    {
        LL_EXTI_ClearFallingFlag_0_31(exti_line);
        drv->falling_call_back();
    }
    else if ((LL_EXTI_IsEnabledRisingTrig_0_31(exti_line) != RESET) &&
             (LL_EXTI_IsActiveRisingFlag_0_31(exti_line) != RESET))
    {
        LL_EXTI_ClearRisingFlag_0_31(exti_line);
        drv->rising_call_back();
    }
}

static inline void mp_port_exti_handler(uint32_t exti_line)
{
    
    #define MP_EXTI(exti_name)                                         \
        #if (MP_EXTI_##exti_name##_LINE >= LL_EXTI_CONFIG_LINE4) && (MP_EXTI_##exti_name##_LINE <= LL_EXTI_CONFIG_LINE14)\
            #ifdef MP_EXTI_##exti_name##_FALLING_CALLBACK              \
            if ((LL_EXTI_IsEnabledFallingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveFallingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearFallingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_GPIOEX_IRQ_FALLING_CALLBACK;                   \
            }                                                          \
            #endif                                                     \
            #ifdef MP_EXTI_##exti_name##_RISING_CALLBACK               \
            if ((LL_EXTI_IsEnabledRisingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveRisingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearRisingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_##exti_name##_RISING_CALLBACK;                 \
            }                                                          \
            #endif
        #endif
        
    MP_EXTI_TABLE;
}

static inline void mp_port_exti4_14_handler()
{
    #define MP_EXTI(exti_name)                                         \
        #if (MP_EXTI_##exti_name##_LINE >= LL_EXTI_CONFIG_LINE4) && (MP_EXTI_##exti_name##_LINE <= LL_EXTI_CONFIG_LINE14)\
            #ifdef MP_EXTI_##exti_name##_FALLING_CALLBACK              \
            if ((LL_EXTI_IsEnabledFallingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveFallingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearFallingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_GPIOEX_IRQ_FALLING_CALLBACK;                   \
            }                                                          \
            #endif                                                     \
            #ifdef MP_EXTI_##exti_name##_RISING_CALLBACK               \
            if ((LL_EXTI_IsEnabledRisingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveRisingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearRisingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_##exti_name##_RISING_CALLBACK;                 \
            }                                                          \
            #endif
        #endif
        
    MP_EXTI_TABLE;
    

        //#ifdef MP_EXTI_BP1_FALLING_CALLBACK
        //if ((LL_EXTI_IsEnabledFallingTrig_0_31(MP_EXTI_BP1_LINE) != RESET) &&
            //(LL_EXTI_IsActiveFallingFlag_0_31(MP_EXTI_BP1_LINE) != RESET))
        //{
            //LL_EXTI_ClearFallingFlag_0_31(MP_EXTI_BP1_LINE);
            //MP_EXTI_BP1_FALLING_CALLBACK;
        //}
        //#endif
        //#ifdef MP_EXTI_BP1_RISING_CALLBACK
        //if ((LL_EXTI_IsEnabledRisingTrig_0_31(MP_EXTI_BP1_LINE) != RESET) &&
            //(LL_EXTI_IsActiveRisingFlag_0_31(MP_EXTI_BP1_LINE) != RESET))
        //{
            //LL_EXTI_ClearRisingFlag_0_31(MP_EXTI_BP1_LINE);
            //MP_EXTI_BP1_RISING_CALLBACK;
        //}
        //#endif
}
static inline void mp_port_exti0_1_handler()
{
    #define MP_EXTI(exti_name)                                         \
        #if (MP_EXTI_##exti_name##_LINE >= LL_EXTI_CONFIG_LINE0) && (MP_EXTI_##exti_name##_LINE <= LL_EXTI_CONFIG_LINE1)\
            #ifdef MP_EXTI_##exti_name##_FALLING_CALLBACK              \
            if ((LL_EXTI_IsEnabledFallingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveFallingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearFallingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_##exti_name##_FALLING_CALLBACK;                \
            }                                                          \
            #endif                                                     \
            #ifdef MP_EXTI_##exti_name##_RISING_CALLBACK               \
            if ((LL_EXTI_IsEnabledRisingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveRisingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearRisingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_##exti_name##_RISING_CALLBACK;                 \
            }                                                          \
            #endif
        #endif
        
    MP_EXTI_TABLE;
}
static inline void mp_port_exti2_3_handler()
{
    #define MP_EXTI(exti_name)                                         \
        #if (MP_EXTI_##exti_name##_LINE >= LL_EXTI_CONFIG_LINE2) && (MP_EXTI_##exti_name##_LINE <= LL_EXTI_CONFIG_LINE3)\
            #ifdef MP_EXTI_##exti_name##_FALLING_CALLBACK              \
            if ((LL_EXTI_IsEnabledFallingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveFallingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearFallingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_##exti_name##_FALLING_CALLBACK;                \
            }                                                          \
            #endif                                                     \
            #ifdef MP_EXTI_##exti_name##_RISING_CALLBACK               \
            if ((LL_EXTI_IsEnabledRisingTrig_0_31(MP_EXTI_##exti_name##_LINE) != RESET) &&\
                (LL_EXTI_IsActiveRisingFlag_0_31(MP_EXTI_##exti_name##_LINE) != RESET))\
            {                                                          \
                LL_EXTI_ClearRisingFlag_0_31(MP_EXTI_##exti_name##_LINE);\
                MP_EXTI_##exti_name##_RISING_CALLBACK;                 \
            }                                                          \
            #endif
        #endif
        
    MP_EXTI_TABLE;
}

#endif // MP_FREERTOS_STM32G0XX_EXTI_H
