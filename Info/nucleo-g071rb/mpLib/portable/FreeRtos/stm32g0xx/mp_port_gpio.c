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

// Implemented functions -------------------------------------------------------
int mp_port_gpio_init(mp_port_gpio_t * dev, GPIO_TypeDef * pripheral)
{
    dev->gpiox = pripheral;
    _mp_gpio_init_table(dev);
    return 0;
}

int mp_port_gpio_deinit(mp_port_gpio_t * dev)
{
    _mp_gpio_deinit_table(dev);
    return 0;
}

int mp_port_gpio_set_value(mp_port_gpio_t * dev,    unsigned int pinmask,
                                                    int value)
{
    uint32_t bsrr = pinmask<<16; // Reset pinmask
    bsrr |= value?pinmask:0x00000; // Set pinmask
    WRITE_REG(dev->gpiox->BSRR, bsrr);
    return 0;
}

int mp_port_gpio_get_value(mp_port_gpio_t * dev, unsigned int pinmask)
{
    return (int)LL_GPIO_IsInputPinSet(dev->gpiox, (uint32_t)pinmask);
}
