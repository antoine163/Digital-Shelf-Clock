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

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_gpio_t parent;
    GPIO_TypeDef * gpiox;
}mp_port_gpio_t;

// Prototype functions ---------------------------------------------------------
int mp_port_gpio_init(mp_port_gpio_t * dev, GPIO_TypeDef * pripheral);
int mp_port_gpio_deinit(mp_port_gpio_t * dev);
int mp_port_gpio_set_value(mp_port_gpio_t * dev,    unsigned int pinmask,
                                                    int value);
int mp_port_gpio_get_value( mp_port_gpio_t * dev,    unsigned int pinmask);

// Static inline functions -----------------------------------------------------
static inline int mp_port_gpio_set_output(  mp_port_gpio_t * dev,
                                            unsigned int pinmask,
                                            mp_gpio_type_t type, 
                                            mp_gpio_pull_t pull, 
                                            int value)
{
    GPIO_TypeDef * gpiox = dev->gpiox;
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

    mp_port_gpio_set_value(dev, pinmask, value);

    LL_GPIO_SetPinOutputType(gpiox, pinmask, ll_type);
    
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

    #define _MP_GPIO_PINMASK_SET_PULL_AND_MODE(pin)                    \
        if ( pinmask&LL_GPIO_PIN_##pin )                               \
        {                                                              \
            shift = (2*pin);                                           \
            regsmask |= 0x3 << shift;                                  \
            pupdr |= ll_pull << shift;                                 \
            moder |= LL_GPIO_MODE_OUTPUT << shift;                     \
        }
    
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(0);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(1);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(2);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(3);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(4);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(5);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(6);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(7);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(8);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(9);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(10);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(11);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(12);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(13);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(14);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(15);
    #undef _MP_GPIO_PINMASK_SET_PULL_AND_MODE
    
    // Set values to pupdr and moder registers
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_port_gpio_set_input(   mp_port_gpio_t * dev,
                                            unsigned int pinmask,
                                            mp_gpio_pull_t pull)
{
    uint32_t ll_pull;
    
    switch (pull)
    {
        case MP_GPIO_PULL_NO:   ll_pull = LL_GPIO_PULL_NO; break;
        case MP_GPIO_PULL_UP:   ll_pull = LL_GPIO_PULL_UP; break;
        case MP_GPIO_PULL_DOWN: ll_pull = LL_GPIO_PULL_DOWN; break;
        default: return -1; // Todo: définir un code d'erreur
    }
    
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

    #define _MP_GPIO_PINMASK_SET_PULL_AND_MODE(pin)                    \
        if ( pinmask&LL_GPIO_PIN_##pin )                               \
        {                                                              \
            shift = (2*pin);                                           \
            regsmask |= 0x3 << shift;                                  \
            pupdr |= ll_pull << shift;                                 \
            moder |= LL_GPIO_MODE_INPUT << shift;                      \
        }
    
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(0);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(1);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(2);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(3);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(4);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(5);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(6);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(7);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(8);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(9);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(10);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(11);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(12);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(13);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(14);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(15);
    #undef _MP_GPIO_PINMASK_SET_PULL_AND_MODE
    
    // Set values to pupdr and moder registers
    GPIO_TypeDef * gpiox = dev->gpiox;
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_port_gpio_set_default( mp_port_gpio_t * dev,
                                            unsigned int pinmask)
{
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

    #define _MP_GPIO_PINMASK_SET_PULL_AND_MODE(pin)                    \
        if ( pinmask&LL_GPIO_PIN_##pin )                               \
        {                                                              \
            shift = (2*pin);                                           \
            regsmask |= 0x3 << shift;                                  \
            pupdr |= LL_GPIO_PULL_NO << shift;                         \
            moder |= LL_GPIO_MODE_ANALOG << shift;                     \
        }
    
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(0);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(1);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(2);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(3);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(4);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(5);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(6);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(7);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(8);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(9);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(10);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(11);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(12);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(13);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(14);
    _MP_GPIO_PINMASK_SET_PULL_AND_MODE(15);
    #undef _MP_GPIO_PINMASK_SET_PULL_AND_MODE
    
    // Set values to pupdr and moder registers
    GPIO_TypeDef *gpiox = dev->gpiox;
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_port_gpio_ctl( mp_port_gpio_t * dev,
                                    unsigned int pinmask,
                                    int request,
                                    va_list ap)
{
    (void)dev;
    (void)pinmask;
    (void)request;
    (void)ap;
    
    return -1;
}

static inline int mp_port_gpio_reset(   mp_port_gpio_t * dev,
                                        unsigned int pinmask)
{
    LL_GPIO_ResetOutputPin(dev->gpiox, pinmask);
    return 0;
}

static inline int mp_port_gpio_set( mp_port_gpio_t * dev, 
                                    unsigned int pinmask)
{
    (void)dev;
    LL_GPIO_SetOutputPin(dev->gpiox, pinmask);
    return 0;
}

static inline int mp_port_gpio_toggle(  mp_port_gpio_t * dev,
                                        unsigned int pinmask)
{
    (void)dev;
    LL_GPIO_TogglePin(dev->gpiox, pinmask);
    return 0;
}

#endif // MP_FREERTOS_STM32G0XX_ADC_H
