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

#ifndef MP_FREERTOS_STM32G0XX_GPIO_H
#define MP_FREERTOS_STM32G0XX_GPIO_H

// Include ---------------------------------------------------------------------
#include "stm32g0xx.h"
// Don't include "mp/drivers/gpio.h" here. It is "mp/drivers/gpio.h" which
// include "mp_port_gpio.h" after to have declare enum, strucur, typdef, ...

#include "stm32g0xx_ll_gpio.h"

// Define macro ------------------------------------------------------------
#define MP_GPIO_PORT(drv, port, pin) drv,                            /* drv  */\
                                     (unsigned int)GPIO##port,       /* port */\
                                     (unsigned int)LL_GPIO_PIN_##pin /* pin  */

// Structure -------------------------------------------------------------------
typedef struct
{
    int empty;
}mp_port_gpio_t;

// Prototype functions ---------------------------------------------------------
int mp_port_gpio_init(mp_port_gpio_t *drv, void *unused);
int mp_port_gpio_deinit(mp_port_gpio_t *drv);

static inline int mp_port_gpio_set_output(mp_port_gpio_t *drv, unsigned int port, unsigned int pin, 
                                                        mp_gpio_type_t type, 
                                                        mp_gpio_pull_t pull, 
                                                        unsigned int value)
{
    (void)drv;
    uint32_t ll_type;
    uint32_t ll_pull;
    
    switch (type)
    {
        case MP_GPIO_TYPE_PUSH_PULL:    ll_type = LL_GPIO_OUTPUT_PUSHPULL; break;
        case MP_GPIO_TYPE_OPEN_DRAIN:   ll_type = LL_GPIO_OUTPUT_OPENDRAIN; break;
        default: return -1; // Todo: définir un code d'erreur
    }
    
    switch (pull)
    {
        case MP_GPIO_PULL_NO:   ll_pull = LL_GPIO_PULL_NO; break;
        case MP_GPIO_PULL_UP:   ll_pull = LL_GPIO_PULL_UP; break;
        case MP_GPIO_PULL_DOWN: ll_pull = LL_GPIO_PULL_DOWN; break;
        default: return -1; // Todo: définir un code d'erreur
    }
    
    if (value == 0)
        LL_GPIO_ResetOutputPin((GPIO_TypeDef*)port, pin);
    else
        LL_GPIO_SetOutputPin((GPIO_TypeDef*)port, pin);
    
    LL_GPIO_SetPinOutputType((GPIO_TypeDef*)port, pin, ll_type);
    LL_GPIO_SetPinPull((GPIO_TypeDef*)port, pin, ll_pull);
    LL_GPIO_SetPinMode((GPIO_TypeDef*)port, pin, LL_GPIO_MODE_OUTPUT);
    
    return 0;
}

static inline int mp_port_gpio_set_input(mp_port_gpio_t *drv, unsigned int port, unsigned int pin,
                                             mp_gpio_pull_t pull)
{
    (void)drv;
    uint32_t ll_pull;
    
    switch (pull)
    {
        case MP_GPIO_PULL_NO:   ll_pull = LL_GPIO_PULL_NO; break;
        case MP_GPIO_PULL_UP:   ll_pull = LL_GPIO_PULL_UP; break;
        case MP_GPIO_PULL_DOWN: ll_pull = LL_GPIO_PULL_DOWN; break;
        default: return -1; // Todo: définir un code d'erreur
    }
    
    LL_GPIO_SetPinPull((GPIO_TypeDef*)port, pin, ll_pull);
    LL_GPIO_SetPinMode((GPIO_TypeDef*)port, pin, LL_GPIO_MODE_INPUT);
    
    return 0;
}

static inline int mp_port_gpio_set_default(mp_port_gpio_t *drv, unsigned int port, unsigned int pin)
{
    (void)drv;
    LL_GPIO_SetPinPull((GPIO_TypeDef*)port, pin, LL_GPIO_PULL_NO);
    LL_GPIO_SetPinMode((GPIO_TypeDef*)port, pin, LL_GPIO_MODE_ANALOG);
    return 0;
}

static inline int mp_port_gpio_ctl(mp_port_gpio_t *drv, unsigned int port, unsigned int pin,
                                              int request, va_list ap)
{
    (void)drv;
    (void)port;
    (void)pin;
    (void)request;
    (void)ap;
    
    return -1;
}

static inline int mp_port_gpio_reset(mp_port_gpio_t *drv, unsigned int port, unsigned int pin)
{
    (void)drv;
    LL_GPIO_ResetOutputPin((GPIO_TypeDef*)port, pin);
    return 0;
}

static inline int mp_port_gpio_set(mp_port_gpio_t *drv, unsigned int port, unsigned int pin)
{
    (void)drv;
    LL_GPIO_SetOutputPin((GPIO_TypeDef*)port, pin);
    return 0;
}

static inline int mp_port_gpio_toggle(mp_port_gpio_t *drv, unsigned int port, unsigned int pin)
{
    (void)drv;
    LL_GPIO_TogglePin((GPIO_TypeDef*)port, pin);
    return 0;
}

static inline int mp_port_gpio_set_value(mp_port_gpio_t *drv, unsigned int port, unsigned int pin,
                                                                unsigned int value)
{
    (void)drv;
    
    GPIO_TypeDef *st_gpiox = (GPIO_TypeDef*)port;
    uint32_t bsrr = pin<<16; // Reset pin
    bsrr |= pin * value; // Set pin
    
    st_gpiox->BSRR = bsrr;
    
    return 0;
}

static inline int mp_port_gpio_get_value(mp_port_gpio_t *drv, unsigned int port, unsigned int pin)
{
    (void)drv;
    return (int)LL_GPIO_IsInputPinSet((GPIO_TypeDef*)port, (uint32_t)pin);
}

#endif // MP_FREERTOS_STM32G0XX_ADC_H
