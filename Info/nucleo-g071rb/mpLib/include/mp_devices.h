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
  
#ifndef MP_DEVICES_H
#define MP_DEVICES_H

// Include ---------------------------------------------------------------------
#include "mpDevicesTable.h"
#include "mp_drivers.h"


#define MP_DEVICE(dev)           ((mp_device_t *)dev)
#define MP_DEVICE_GET(devid)     mp_device_get(devid)

#define MP_DEVICE_ID(devid)         ( devid & 0x0000ffff )
#define MP_DEVICE_TYPE(devid)     ( ( devid & 0xffff0000 ) >> 16 )


// Enum ------------------------------------------------------------------------

// Build devices id
typedef enum
{
    // Undefine all devices macro.
    #include "mp_undef_all_dev.h"
    
    #undef MP_DEV_GPIO
    #define MP_DEV_GPIO(device, driver, peripheral)                    \
        MP_DEVICE_ID_##device,                                         \
        device =    (MP_DRIVER_TYPE_GPIO_##driver << 16) |             \
                    MP_DEVICE_ID_##device,                             \
        _MP_DEVICE_ID_##device = MP_DEVICE_ID_##device,
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        MP_DEVICE_ID_##device,                                         \
        device =    (MP_DRIVER_TYPE_UART_##driver << 16) |             \
                    MP_DEVICE_ID_##device,                             \
        _MP_DEVICE_ID_##device = MP_DEVICE_ID_##device,
    
    MP_DEVICES_TABLE
}mp_device_id_t;

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_device_id_t const devid;
    
    // option semaphor
    // sem protect;
} mp_device_t;

// extern global variable ------------------------------------------------------
extern mp_device_t * const _mp_devices_table[];

// Static inline function ------------------------------------------------------
static inline mp_device_t * mp_device_get(mp_device_id_t devid)
{
    return _mp_devices_table[MP_DEVICE_ID(devid)];
}

#endif // MP_DEVICES_H

