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
void (*_mp_gpio_port_extix_callback[16])(mp_gpio_trigger_t) = {NULL};

// Implemented functions -------------------------------------------------------

int mp_gpio_port_init(mp_device_id_t devid)
{
    _mp_gpio_init_table(devid);
    MP_DEVICE_GET(devid)->isInit = 1;
    return 0;
}

int mp_gpio_port_deinit(mp_device_id_t devid)
{
    (void)devid;
    _mp_gpio_deinit_table(devid);
    MP_DEVICE_GET(devid)->isInit = 0;
    return 0;
}

// Todo: a deplaser dans la table des IT
void EXTI4_15_IRQHandler()
{
    mp_gpio_port_4_15_handler();
}

void EXTI2_3_IRQHandler()
{
    mp_gpio_port_2_3_handler();
}

void EXTI0_1_IRQHandler()
{
    mp_gpio_port_0_1_handler();
}
