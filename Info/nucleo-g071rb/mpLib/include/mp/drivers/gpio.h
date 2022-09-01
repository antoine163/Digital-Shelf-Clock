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

#ifndef MP_GPIO_H
#define MP_GPIO_H

// Include ---------------------------------------------------------------------
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#include "mp_preprocessor.h"
#include "mp_drivers.h"
#include "mp_devices.h"
#include "mpHardMap.h"

// Define macro ----------------------------------------------------------------
#define _MP_GPIO_PIN_TO_MASK(pin) (1<<pin) |
#define MP_GPIO_PIN(devid, pin, ...)                                   \
    /* Device id  */                                                   \
    devid,                                                             \
    /* Build mask from all pins  */                                    \
    EVAL(MAP(_MP_GPIO_PIN_TO_MASK, pin, __VA_ARGS__)) 0

#define MP_GPIO_LEVEL_LOW       0x00000000
#define MP_GPIO_LEVEL_HIGH      0xffffffff
#define MP_GPIO_LEVEL_L         0x00000000
#define MP_GPIO_LEVEL_H         0xffffffff
#define MP_GPIO_LEVEL_0         0x00000000
#define MP_GPIO_LEVEL_1         0xffffffff
#define MP_GPIO_LEVEL_MASK(level)    level

#define MP_GPIO_OUT(gpio, type, pull, level)                           \
        mp_gpio_output(gpio,    MP_GPIO_TYPE_##type,                   \
                                MP_GPIO_PULL_##pull,                   \
                                MP_GPIO_LEVEL_##level)
                                
#define MP_GPIO_IN(gpio, pull)                                         \
        mp_gpio_input(gpio, MP_GPIO_PULL_##pull)

// Enum ------------------------------------------------------------------------
typedef enum
{
    MP_GPIO_TYPE_PUSH_PULL,
    MP_GPIO_TYPE_OPEN_DRAIN,
    MP_GPIO_TYPE_OPEN_SOURCE
}mp_gpio_type_t;

typedef enum
{
    MP_GPIO_PULL_NO,
    MP_GPIO_PULL_UP,
    MP_GPIO_PULL_DOWN
}mp_gpio_pull_t;

typedef enum
{
    MP_GPIO_TRIGGER_UNKNOWN             = 0x00,
    MP_GPIO_TRIGGER_FALLING             = 0x01,
    MP_GPIO_TRIGGER_RISING              = 0x02,
    MP_GPIO_TRIGGER_FALLING_AND_RISING  = 0x03
}mp_gpio_trigger_t;

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_device_t device_parent;
} mp_gpio_t;

// Include ---------------------------------------------------------------------
//#warning "Todo fair comme dans uart"
#include "mpDevicesTable.h"
#include "mp_gpio_port.h"


// Include GPIO drivers from devices, know by mpLib
//#include "..."

// Include additional GPIO drivers unknown by mpLib
#ifdef MP_ADD_GPIO_DRIVER_HEADER
    #include MP_ADD_GPIO_DRIVER_HEADER
#endif // MP_ADD_GPIO_DRIVER_HEADER

// Extern global variables -----------------------------------------------------

// Public static 'virtual' functions -------------------------------------------

// Define all device macro to empty.
// Note: Only MP_DEV_GPIO will be redefine in this file for each functions.
#include "mp_def_empty_dev.h"

static inline int mp_gpio_init(mp_device_id_t devid)
{
    int ret = -1;

    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_init(devid);                      \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

// Warning: mp_gpio_deinit() ne desctive pas les IT dégà activer. Il faut au 
// prealable les desactiver avec mp_gpio_disableIt() evant d'apeler
// mp_gpio_deinit()
static inline int mp_gpio_deinit(mp_device_id_t devid)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_deinit(devid);                    \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

static inline int mp_gpio_output(mp_device_id_t devid,  unsigned int pinmask,
                                                        mp_gpio_type_t type, 
                                                        mp_gpio_pull_t pull, 
                                                        unsigned int level)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_output(devid, pinmask,            \
                                            type, pull, level);        \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

static inline int mp_gpio_input(mp_device_id_t devid,   unsigned int pinmask,
                                                        mp_gpio_pull_t pull)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_input(devid, pinmask, pull);      \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

static inline int mp_gpio_default(mp_device_id_t devid, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_default(devid, pinmask);          \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

static inline int mp_gpio_setLevel(mp_device_id_t devid,    unsigned int pinmask,
                                                            unsigned int level)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_setLevel(devid, pinmask, level);  \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}
//TODO fair mp_gpio_getInLevel
//TODO fair mp_gpio_getOutLevel
static inline int mp_gpio_getLevel(mp_device_id_t devid, unsigned int pinmask,
                                                unsigned int * level)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_getLevel(devid, pinmask, level);  \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

static inline int mp_gpio_ctl(mp_device_id_t devid,  unsigned int pinmask,
                                                int request, ...)
{
    int ret = -1;
    va_list ap;
    va_start(ap, request);
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_ctl(devid, pinmask, request, ap); \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    va_end(ap);
    return ret;
}

static inline int mp_gpio_down(mp_device_id_t devid, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_down(devid, pinmask);             \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

 static inline int mp_gpio_up(mp_device_id_t devid, unsigned int pinmask)
{
    int ret = -1;

    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_up(devid, pinmask);               \
        }
    
    if(0){}
    MP_DEVICES_TABLE

    return ret;
}

static inline int mp_gpio_toggle(mp_device_id_t devid, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_toggle(devid, pinmask);           \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

// Note: mp_gpio_deinit() ne desctive pas les IT dégà activer. Il faut au 
// prealable les desactiver avec mp_gpio_disableIt() evant d'apeler
// mp_gpio_deinit()
static inline int mp_gpio_enableIsr(mp_device_id_t devid, unsigned int pinmask,
                                    mp_gpio_trigger_t trigger,
                                    void (*isr)(mp_gpio_trigger_t))
{
    int ret = -1;

    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_enableIt(                         \
                devid, pinmask, trigger, isr);                         \
        }
    
    if(0){}
    MP_DEVICES_TABLE

    return ret;
}

static inline int mp_gpio_disableIsr(mp_device_id_t devid, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_GPIO_##driver)\
        {                                                              \
            ret = mp_gpio_##driver##_disableIsr(devid, pinmask);       \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    return ret;
}

// Public static functions -----------------------------------------------------
static inline int mp_gpio_setValue(mp_device_id_t devid,    unsigned int pinmask,
                                                            unsigned int value)
{
    return mp_gpio_setLevel(devid, pinmask, value ? pinmask : 0);
}

static inline int mp_gpio_getValue(mp_device_id_t devid, unsigned int pinmask)
{
    unsigned int value;
    int ret = -1;
    
    ret = mp_gpio_getLevel(devid, pinmask, &value);
    
    if (ret >= 0)
    {
        ret = value ? 1 : 0;
    }
    
    return ret;
}


#endif // MP_GPIO_H
