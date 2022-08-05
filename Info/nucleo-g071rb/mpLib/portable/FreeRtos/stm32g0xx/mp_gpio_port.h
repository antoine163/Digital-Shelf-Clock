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

#ifndef MP_GPIO_PORT_H
#define MP_GPIO_PORT_H

// Include ---------------------------------------------------------------------
#include "stm32g0xx.h"
// Don't include "mp/drivers/gpio.h" here. It is "mp/drivers/gpio.h" which
// include "mp_port_gpio.h" after to have declare enum, strucur, typdef, ...

#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_exti.h"


#define MP_PORT_GPIO(dev)           ((mp_gpio_port_t *)dev)
#define MP_PORT_GPIO_GET(devid)     MP_PORT_GPIO(mp_device_get(devid))

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_gpio_t gpio_parent;
    GPIO_TypeDef * const gpiox;
}mp_gpio_port_t;

// Extern protected global variables -------------------------------------------
extern void (*_mp_port_gpio_extix_callback[16])(mp_gpio_trigger_t);

// Prototype functions ---------------------------------------------------------
int mp_port_gpio_init(mp_device_id_t devid);
int mp_port_gpio_deinit(mp_device_id_t devid);

int mp_port_gpio_enableIt(  mp_device_id_t devid, unsigned int pinmask,
                            mp_gpio_trigger_t trigger,
                            void (*callback)(mp_gpio_trigger_t));

// Static inline functions -----------------------------------------------------
static inline int mp_port_gpio_setLevel(mp_device_id_t devid,
                                        unsigned int pinmask,
                                        unsigned int level)
{
    uint32_t bsrr = pinmask<<16; // Reset pinmask
    bsrr |= level&pinmask; // Set pinmask
    
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    WRITE_REG(dev->gpiox->BSRR, bsrr);
    return 0;
}

static inline int mp_port_gpio_getLevel(mp_device_id_t devid,
                                    unsigned int pinmask,
                                    unsigned int * level)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    *level = LL_GPIO_ReadInputPort(dev->gpiox)&pinmask;
    return 0;
}

static inline int mp_port_gpio_output(  mp_device_id_t devid,
                                        unsigned int pinmask,
                                        mp_gpio_type_t type, 
                                        mp_gpio_pull_t pull, 
                                        unsigned int level)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
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

    // Write default value before configure gpio output type
    mp_port_gpio_setLevel(devid, pinmask, level);
    LL_GPIO_SetPinOutputType(gpiox, pinmask, ll_type);
    
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

# if 1 // 2420
    #define _MP_GPIO_PINMASK_SET_PULL_AND_MODE(pin)                    \
        if ( pinmask & (1<<pin) )                                      \
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
#else // 2640
    for (uint32_t pin = 0; pin < 16; pin++)
    {
        if ( pinmask & (1<<pin) )
        {
            shift = (2*pin);
            regsmask |= 0x3 << shift;
            pupdr |= ll_pull << shift;
            moder |= LL_GPIO_MODE_OUTPUT << shift;
        }
    }
#endif

    // Set values to pupdr and moder registers
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_port_gpio_input(   mp_device_id_t devid,
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

#if 1 // 2420
    #define _MP_GPIO_PINMASK_SET_PULL_AND_MODE(pin)                    \
        if ( pinmask & (1<<pin) )                                      \
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
#else // 2480
    for (uint32_t pin = 0; pin < 16; pin++)
    {
        if ( pinmask & (1<<pin) )
        {
            shift = (2*pin);
            regsmask |= 0x3 << shift;
            pupdr |= ll_pull << shift;
            moder |= LL_GPIO_MODE_INPUT << shift;
        }
    }
#endif
    
    // Set values to pupdr and moder registers
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    GPIO_TypeDef * gpiox = dev->gpiox;
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_port_gpio_default( mp_device_id_t devid,
                                        unsigned int pinmask)
{
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

#if 0 // 2420
    #define _MP_GPIO_PINMASK_SET_PULL_AND_MODE(pin)                    \
        if ( pinmask & (1<<pin) )                                      \
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
#else // 2420
    for (uint32_t pin = 0; pin < 16; pin++)
    {
        if ( pinmask & (1<<pin) )
        {
            shift = (2*pin);
            regsmask |= 0x3 << shift;
            pupdr |= LL_GPIO_PULL_NO << shift;
            moder |= LL_GPIO_MODE_ANALOG << shift;
        }
    }
#endif

    // Set values to pupdr and moder registers
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    GPIO_TypeDef *gpiox = dev->gpiox;
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_port_gpio_ctl( mp_device_id_t devid,
                                    unsigned int pinmask,
                                    int request,
                                    va_list ap)
{
    (void)devid;
    (void)pinmask;
    (void)request;
    (void)ap;
    
    return -1;
}

static inline int mp_port_gpio_down(    mp_device_id_t devid,
                                        unsigned int pinmask)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    LL_GPIO_ResetOutputPin(dev->gpiox, pinmask);
    return 0;
}

static inline int mp_port_gpio_up(  mp_device_id_t devid, 
                                    unsigned int pinmask)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    LL_GPIO_SetOutputPin(dev->gpiox, pinmask);
    return 0;
}

static inline int mp_gpio_port_toggle(  mp_device_id_t devid,
                                        unsigned int pinmask)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    LL_GPIO_TogglePin(dev->gpiox, pinmask);
    return 0;
}

static inline int mp_port_gpio_disableIt(   mp_device_id_t devid,
                                            unsigned int pinmask)
{
    (void)devid;
    LL_EXTI_DisableIT_0_31((uint32_t)pinmask);
    LL_EXTI_DisableFallingTrig_0_31((uint32_t)pinmask);
    LL_EXTI_DisableRisingTrig_0_31((uint32_t)pinmask);
    return 0;
}

__attribute__((always_inline))
static inline void mp_port_gpio4_15_handler()
{     
    // 0x0000fff0 is mask from LL_EXTI_LINE_4 to LL_EXTI_LINE_15
    uint32_t enabledFallingTrig = READ_BIT(EXTI->FTSR1, 0x0000fff0);
    uint32_t enabledRisingTrig = READ_BIT(EXTI->RTSR1, 0x0000fff0);
    
    uint32_t isActiveFallingFlag = READ_BIT(EXTI->FPR1, enabledFallingTrig);
    uint32_t isActiveRisingFlag = READ_BIT(EXTI->RPR1, enabledRisingTrig);
    
    LL_EXTI_ClearFallingFlag_0_31(isActiveFallingFlag);
    LL_EXTI_ClearRisingFlag_0_31(isActiveRisingFlag);
    
    for (unsigned int i = 1; i < 16; i++)
    {
        mp_gpio_trigger_t trigger = 0x00;
        trigger |= (isActiveFallingFlag >> i) & 1;
        trigger |= ((isActiveRisingFlag >> i) & 1 ) << 1;
        
        if (trigger)
            _mp_port_gpio_extix_callback[i](trigger);
    }
}

#endif // MP_GPIO_PORT_H
