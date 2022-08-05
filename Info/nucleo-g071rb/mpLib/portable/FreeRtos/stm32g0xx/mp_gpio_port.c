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
#include "mp/drivers/gpio.h"

// Protected global variables --------------------------------------------------
void (*_mp_port_gpio_extix_callback[16])(mp_gpio_trigger_t) = {NULL};

// Implemented functions -------------------------------------------------------
int mp_port_gpio_init(mp_device_id_t devid)
{
    (void)devid;
    //dev->gpiox = pripheral;
    //_mp_gpio_init_table( (mp_gpio_t * const)dev );
    return 0;
}

int mp_port_gpio_deinit(mp_device_id_t devid)
{
    (void)devid;
    //_mp_gpio_deinit_table( (mp_gpio_t * const)dev );
    return 0;
}

int mp_port_gpio_enableIt(  mp_device_id_t devid, unsigned int pinmask,
                            mp_gpio_trigger_t trigger,
                            void (*callback)(mp_gpio_trigger_t))
{  
    if (LL_EXTI_IsEnabledIT_0_31((uint32_t)pinmask))
        return -1;
    
    // Compute the port value from gpio base
    // - The port match with the Port argument of LL_EXTI_SetEXTISource().
    // - The gpio base match with GPIOx_BASE (with x = A, B, C, D, E or F).
    mp_gpio_port_t * dev = (mp_gpio_port_t *)mp_device_get(devid);
    uint32_t port = ( ( (uint32_t)dev->gpiox) >> 10 ) & 0x7;
            
    // Find the pin number from pinmask to configure external interruption
    for (unsigned int i = 0; i < 16; i++)
    {
        if ((unsigned int)(1<<i) == pinmask)
        {
            _mp_port_gpio_extix_callback[i] = callback;

            // Configure source input for the EXTI external interrupt.
            // See LL_EXTI_SetEXTISource()
            uint32_t clearmask = 0xff << (i & 0x03);
            uint32_t setmask = port << (i & 0x03);
            MODIFY_REG(EXTI->EXTICR[i>>2], clearmask, setmask);
            
            if (trigger & MP_GPIO_TRIGGER_FALLING)
                LL_EXTI_EnableFallingTrig_0_31((uint32_t)pinmask);
            if (trigger & MP_GPIO_TRIGGER_RISING)
                LL_EXTI_EnableRisingTrig_0_31((uint32_t)pinmask);
            
            LL_EXTI_EnableIT_0_31((uint32_t)pinmask);
            return 0;
        }
    }
    
    return -1;
}


// Todo a deplaser dans la table des IT
void EXTI4_15_IRQHandler()
{
    mp_port_gpio4_15_handler();
}
