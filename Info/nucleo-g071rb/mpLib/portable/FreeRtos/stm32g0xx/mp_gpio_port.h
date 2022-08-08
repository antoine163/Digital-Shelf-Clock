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
// include "mp_gpio_port.h" after to have declare enum, strucur, typdef, ...

#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_bus.h"

// Define macro ----------------------------------------------------------------
#define MP_PORT_GPIO(dev)           ((mp_gpio_port_t *)dev)
#define MP_PORT_GPIO_GET(devid)     MP_PORT_GPIO(mp_device_get(devid))

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_gpio_t gpio_parent;
    GPIO_TypeDef * const gpiox;
}mp_gpio_port_t;

// Extern protected global variables -------------------------------------------
extern void (*_mp_gpio_port_extix_isr[16])(mp_gpio_trigger_t);

// Prototype functions ---------------------------------------------------------
static inline int mp_gpio_port_setLevel(mp_device_id_t devid,
                                        unsigned int pinmask,
                                        unsigned int level);

// Static inline functions -----------------------------------------------------
static inline int mp_gpio_port_init(mp_device_id_t devid)
{
    GPIO_TypeDef * gpiox = MP_PORT_GPIO_GET(devid)->gpiox;
    uint32_t periphs = 0;
    if (gpiox == GPIOA)
        periphs = LL_IOP_GRP1_PERIPH_GPIOA;
    else if (gpiox == GPIOB)
        periphs = LL_IOP_GRP1_PERIPH_GPIOB;
    else if (gpiox == GPIOC)
        periphs = LL_IOP_GRP1_PERIPH_GPIOC;
    else if (gpiox == GPIOD)
        periphs = LL_IOP_GRP1_PERIPH_GPIOD;
    
    #ifdef GPIOE
    else if (gpiox == GPIOE)
        periphs = LL_IOP_GRP1_PERIPH_GPIOE;
    #endif
    
    #ifdef GPIOE
    else if (gpiox == GPIOF)
        periphs = LL_IOP_GRP1_PERIPH_GPIOF;
    #endif

    LL_IOP_GRP1_EnableClock(periphs);
    MP_DEVICE_GET(devid)->isInit = 1;
    return 0;
}

static inline int mp_gpio_port_deinit(mp_device_id_t devid)
{
    MP_DEVICE_GET(devid)->isInit = 0;
    return 0;
}

static inline int mp_gpio_port_output(  mp_device_id_t devid,
                                        unsigned int pinmask,
                                        mp_gpio_type_t type, 
                                        mp_gpio_pull_t pull, 
                                        unsigned int level)
{
    GPIO_TypeDef * gpiox = MP_PORT_GPIO_GET(devid)->gpiox;
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
    mp_gpio_port_setLevel(devid, pinmask, level);
    LL_GPIO_SetPinOutputType(gpiox, pinmask, ll_type);
    
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

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

    // Set values to pupdr and moder registers
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_gpio_port_input(   mp_device_id_t devid,
                                        unsigned int pinmask,
                                        mp_gpio_pull_t pull)
{
    GPIO_TypeDef * gpiox = MP_PORT_GPIO_GET(devid)->gpiox;
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
    
    // Set values to pupdr and moder registers
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_gpio_port_default( mp_device_id_t devid,
                                        unsigned int pinmask)
{
    GPIO_TypeDef * gpiox = MP_PORT_GPIO_GET(devid)->gpiox;
    
    // Prepare pupdr and moder registers mask and values.
    uint32_t regsmask = 0;
    uint32_t pupdr = 0;
    uint32_t moder = 0;
    uint32_t shift;

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

    // Set values to pupdr and moder registers
    MODIFY_REG(gpiox->PUPDR, regsmask, pupdr);
    MODIFY_REG(gpiox->MODER, regsmask, moder);
    
    return 0;
}

static inline int mp_gpio_port_setLevel(mp_device_id_t devid,
                                        unsigned int pinmask,
                                        unsigned int level)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    uint32_t bsrr = pinmask<<16; // Reset pinmask
    bsrr |= level&pinmask; // Set pinmask
    WRITE_REG(dev->gpiox->BSRR, bsrr);
    return 0;
}

static inline int mp_gpio_port_getLevel(mp_device_id_t devid,
                                    unsigned int pinmask,
                                    unsigned int * level)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    *level = LL_GPIO_ReadInputPort(dev->gpiox)&pinmask;
    return 0;
}

static inline int mp_gpio_port_ctl( mp_device_id_t devid,
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

static inline int mp_gpio_port_down(    mp_device_id_t devid,
                                        unsigned int pinmask)
{
    mp_gpio_port_t * dev = MP_PORT_GPIO_GET(devid);
    LL_GPIO_ResetOutputPin(dev->gpiox, pinmask);
    return 0;
}

static inline int mp_gpio_port_up(  mp_device_id_t devid, 
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

static inline int mp_gpio_port_enableIt(mp_device_id_t devid,
                                        unsigned int pinmask,
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
    for (int i = 15; i >= 0; i--)
    {
        if ((unsigned int)(1<<i) == pinmask)
        {
            _mp_gpio_port_extix_isr[i] = callback;

            // Configure source input for the EXTI external interrupt.
            // See LL_EXTI_SetEXTISource()
            uint32_t clearmask = 0xff << (i & 0x03) * 8;
            uint32_t setmask = port << (i & 0x03) * 8;
            MODIFY_REG(EXTI->EXTICR[i>>2], clearmask, setmask);
            
            // Clean trigger flags
            LL_EXTI_ClearFallingFlag_0_31((uint32_t)pinmask);
            LL_EXTI_ClearRisingFlag_0_31((uint32_t)pinmask);
            
            // Enable trigger
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

static inline int mp_gpio_port_disableIsr(  mp_device_id_t devid,
                                            unsigned int pinmask)
{
    (void)devid;
    LL_EXTI_DisableIT_0_31((uint32_t)pinmask);
    LL_EXTI_DisableFallingTrig_0_31((uint32_t)pinmask);
    LL_EXTI_DisableRisingTrig_0_31((uint32_t)pinmask);
    return 0;
}

__attribute__((always_inline))
static inline void mp_gpio_port_exti4_15_isr()
{   
    // Get the enabled triggers. See LL_EXTI_IsEnabledFallingTrig_0_31() and 
    // LL_EXTI_IsEnabledRisingTrig_0_31()
    // Note: 0x0000fff0 is mask from LL_EXTI_LINE_4 to LL_EXTI_LINE_15
    uint32_t enabledFallingTrig = READ_BIT(EXTI->FTSR1, 0x0000fff0);
    uint32_t enabledRisingTrig = READ_BIT(EXTI->RTSR1, 0x0000fff0);
    
    // Get the actives flags. See LL_EXTI_IsActiveFallingFlag_0_31() and
    // LL_EXTI_IsActiveRisingFlag_0_31()
    uint32_t isActiveFallingFlag = READ_BIT(EXTI->FPR1, enabledFallingTrig);
    uint32_t isActiveRisingFlag = READ_BIT(EXTI->RPR1, enabledRisingTrig);
    
    // Clean the active flags
    LL_EXTI_ClearFallingFlag_0_31(isActiveFallingFlag);
    LL_EXTI_ClearRisingFlag_0_31(isActiveRisingFlag);
    
    // Call callbacks for each trigged lines/pins from 15 to 4
    mp_gpio_trigger_t trigger;
    isActiveFallingFlag >>= 4;
    isActiveRisingFlag >>= 3;
    for (int i = 11; i >= 0; i--)
    {
        trigger = (isActiveFallingFlag >> i) & MP_GPIO_TRIGGER_FALLING;
        trigger |= (isActiveRisingFlag >> i) & MP_GPIO_TRIGGER_RISING;
        
        if (trigger)
            _mp_gpio_port_extix_isr[i+4](trigger);
    }
}

__attribute__((always_inline))
static inline void mp_gpio_port_exti2_3_isr()
{   
    // Get the enabled triggers. See LL_EXTI_IsEnabledFallingTrig_0_31() and 
    // LL_EXTI_IsEnabledRisingTrig_0_31()
    // Note: 0x0000000c is mask from LL_EXTI_LINE_2 to LL_EXTI_LINE_3
    uint32_t enabledFallingTrig = READ_BIT(EXTI->FTSR1, 0x0000000c);
    uint32_t enabledRisingTrig = READ_BIT(EXTI->RTSR1, 0x0000000c);
    
    // Get the actives flags. See LL_EXTI_IsActiveFallingFlag_0_31() and
    // LL_EXTI_IsActiveRisingFlag_0_31()
    uint32_t isActiveFallingFlag = READ_BIT(EXTI->FPR1, enabledFallingTrig);
    uint32_t isActiveRisingFlag = READ_BIT(EXTI->RPR1, enabledRisingTrig);
    
    // Clean the active flags
    LL_EXTI_ClearFallingFlag_0_31(isActiveFallingFlag);
    LL_EXTI_ClearRisingFlag_0_31(isActiveRisingFlag);
    
    // Call callbacks for each trigged lines/pins from 2 to 3
    mp_gpio_trigger_t trigger;
    
    // Call callbacks for line/pin 2
    trigger  = (isActiveFallingFlag >> 2) & MP_GPIO_TRIGGER_FALLING;
    trigger |= (isActiveRisingFlag >> 1) & MP_GPIO_TRIGGER_RISING;
    if (trigger)
        _mp_gpio_port_extix_isr[2](trigger);
    
    // Call callbacks for line/pin 3
    trigger  = (isActiveFallingFlag >> 3) & MP_GPIO_TRIGGER_FALLING;
    trigger |= (isActiveRisingFlag >> 2) & MP_GPIO_TRIGGER_RISING;
    if (trigger)
        _mp_gpio_port_extix_isr[3](trigger);
}

__attribute__((always_inline))
static inline void mp_gpio_port_exti0_1_isr()
{   
    // Get the enabled triggers. See LL_EXTI_IsEnabledFallingTrig_0_31() and 
    // LL_EXTI_IsEnabledRisingTrig_0_31()
    // Note: 0x00000003 is mask from LL_EXTI_LINE_0 to LL_EXTI_LINE_1
    uint32_t enabledFallingTrig = READ_BIT(EXTI->FTSR1, 0x00000003);
    uint32_t enabledRisingTrig = READ_BIT(EXTI->RTSR1, 0x00000003);
    
    // Get the actives flags. See LL_EXTI_IsActiveFallingFlag_0_31() and
    // LL_EXTI_IsActiveRisingFlag_0_31()
    uint32_t isActiveFallingFlag = READ_BIT(EXTI->FPR1, enabledFallingTrig);
    uint32_t isActiveRisingFlag = READ_BIT(EXTI->RPR1, enabledRisingTrig);
    
    // Clean the active flags
    LL_EXTI_ClearFallingFlag_0_31(isActiveFallingFlag);
    LL_EXTI_ClearRisingFlag_0_31(isActiveRisingFlag);
    
    // Call callbacks for each trigged lines/pins from 0 to 1
    mp_gpio_trigger_t trigger;
    
    // Call callbacks for line/pin 0
    trigger  =  isActiveFallingFlag & MP_GPIO_TRIGGER_FALLING;
    trigger |= (isActiveRisingFlag << 1) & MP_GPIO_TRIGGER_RISING;
    if (trigger)
        _mp_gpio_port_extix_isr[0](trigger);
    
    // Call callbacks for line/pin 1
    trigger  = (isActiveFallingFlag >> 1) & MP_GPIO_TRIGGER_FALLING;
    trigger |=  isActiveRisingFlag & MP_GPIO_TRIGGER_RISING;
    if (trigger)
        _mp_gpio_port_extix_isr[1](trigger);
}

#endif // MP_GPIO_PORT_H
