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

#ifndef MP_DRIVER_GPIO_H
#define MP_DRIVER_GPIO_H

// Include ---------------------------------------------------------------------
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#include "mpHardMap.h"
#include "devices.h"

// Define macro ----------------------------------------------------------------
#define MP_GPIO_PIN(device , pin)   device, (1<<pin)

// Typedef ---------------------------------------------------------------------
typedef struct mp_gpio_s
{
    mp_device_t parent;
} mp_gpio_t;

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


// Protected define macro ------------------------------------------------------
#define _MP_GPIO_EXTRACT_DEVICE(device, pinmask)    device

// Include ---------------------------------------------------------------------
#include "mpDevicesTable.h"
#include "mp_port_gpio.h"

#include "mp_def_empty_dev.h"

// Include GPIO drivers from devices, know by mpLib
//#include "..."

// Include additional GPIO drivers unknown by mpLib
#ifdef MP_ADD_GPIO_DRIVER_HEADER
    #include MP_ADD_GPIO_DRIVER_HEADER
#endif // MP_ADD_GPIO_DRIVER_HEADER

// Extern global variables -----------------------------------------------------
#undef MP_DEV_GPIO
#define MP_DEV_GPIO(device, driver, peripheral)                        \
    extern mp_gpio_t * device;
    
MP_DEVICES_TABLE
#undef MP_DEV_GPIO
#define MP_DEV_GPIO(device, driver, peripheral)

// Public static functions -----------------------------------------------------
static inline int mp_gpio_init(mp_gpio_t * dev)
{
    int ret = -1;
        
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_init((mp_##driver##_gpio_t *)dev, \
                                            peripheral);               \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    if ( ret >= 0 )
        MP_DEVICE(dev)->isInit = 1;
    
    return ret;
}

static inline int mp_gpio_deinit(mp_gpio_t * dev)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_deinit((mp_##driver##_gpio_t *)dev);\
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    if ( ret >= 0 )
        MP_DEVICE(dev)->isInit = 0;
        
    return ret;
}

static inline int mp_gpio_set_output(mp_gpio_t * dev,   unsigned int pinmask,
                                                        mp_gpio_type_t type, 
                                                        mp_gpio_pull_t pull, 
                                                        unsigned int value)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_set_output((mp_##driver##_gpio_t *)dev,\
                                                pinmask, type,         \
                                                pull, value);          \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_set_input(mp_gpio_t * dev,    unsigned int pinmask,
                                                        mp_gpio_pull_t pull)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_set_input((mp_##driver##_gpio_t *)dev,\
                                                pinmask, pull);        \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_set_default(mp_gpio_t * dev, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_set_default((mp_##driver##_gpio_t *)dev,\
                                                    pinmask);          \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_ctl(mp_gpio_t * dev,  unsigned int pinmask,
                                                int request, ...)
{
    int ret = -1;
    va_list ap;
    va_start(ap, request);
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_ctl((mp_##driver##_gpio_t *)dev,  \
                                        pinmask, request, ap);         \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    va_end(ap);
    return ret;
}

static inline int mp_gpio_reset(mp_gpio_t * dev, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_reset((mp_##driver##_gpio_t *)dev,\
                                            pinmask);                  \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_set(mp_gpio_t * dev, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_set((mp_##driver##_gpio_t *)dev,  \
                                        pinmask);                      \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_toggle(mp_gpio_t * dev, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_toggle((mp_##driver##_gpio_t *)dev,\
                                            pinmask);                  \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_set_value(mp_gpio_t * dev,    unsigned int pinmask,
                                                        unsigned int value)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_set_value((mp_##driver##_gpio_t *)dev,\
                                                pinmask, value);       \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

static inline int mp_gpio_get_value(mp_gpio_t * dev, unsigned int pinmask)
{
    int ret = -1;
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        else if (dev == device)                                        \
        {                                                              \
            ret = mp_##driver##_gpio_get_value((mp_##driver##_gpio_t *)dev,\
                                                pinmask);              \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)
    
    return ret;
}

// Protected static functions --------------------------------------------------
static inline void _mp_gpio_init_table(mp_gpio_t *dev)
{
#ifdef MP_GPIO_TABLE
    #define MP_GPIO_OUT(gpio, type, pull, defvalue)                    \
        if (_MP_GPIO_EXTRACT_DEVICE(gpio) == dev)                      \
        {                                                              \
            mp_gpio_set_output(gpio,    MP_GPIO_TYPE_##type,           \
                                        MP_GPIO_PULL_##pull,           \
                                        defvalue);                     \
        }
    #define MP_GPIO_IN(gpio, pull)                                     \
        if (_MP_GPIO_EXTRACT_DEVICE(gpio) == dev)                      \
        {                                                              \
            mp_gpio_set_input(gpio, MP_GPIO_PULL_##pull);              \
        }                                                              \

    MP_GPIO_TABLE
    
    #undef MP_GPIO_OUT
    #undef MP_GPIO_IN
#endif
}

static inline void _mp_gpio_deinit_table(mp_gpio_t * dev)
{
#ifdef MP_GPIO_TABLE
    #define MP_GPIO_OUT(gpio, type, pull, defvalue)                    \
        if (_MP_GPIO_EXTRACT_DEVICE(gpio) == dev)                      \
        {                                                              \
            mp_gpio_set_default(gpio);                                 \
        }
    #define MP_GPIO_IN(gpio, pull)                                     \
        if (_MP_GPIO_EXTRACT_DEVICE(gpio) == dev)                      \
        {                                                              \
            mp_gpio_set_default(gpio);                                 \
        }                                                              \

    MP_GPIO_TABLE
    
    #undef MP_GPIO_OUT
    #undef MP_GPIO_IN
#endif
}


#endif // MP_DRIVER_GPIO_H
