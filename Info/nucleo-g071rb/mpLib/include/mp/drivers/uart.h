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

#ifndef MP_UART_H
#define MP_UART_H

// Include ---------------------------------------------------------------------
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#include "mp_drivers.h"
#include "mp_devices.h"
#include "mp/drivers/tick.h"

// Enum ------------------------------------------------------------------------
typedef enum
{
    MP_UART_BAUDRATE_300     = 600,     //!< 600 bps.
    MP_UART_BAUDRATE_1200    = 1200,    //!< 1200 bps.
    MP_UART_BAUDRATE_2400    = 2400,    //!< 2400 bps.
    MP_UART_BAUDRATE_4800    = 4800,    //!< 4800 bps.
    MP_UART_BAUDRATE_9600    = 9600,    //!< 9600 bps.
    MP_UART_BAUDRATE_19200   = 19200,   //!< 19200 bps.
    MP_UART_BAUDRATE_38400   = 38400,   //!< 38400 bps.
    MP_UART_BAUDRATE_57600   = 57600,   //!< 57600 bps.
    MP_UART_BAUDRATE_115200  = 115200,  //!< 115200 bps.
    MP_UART_BAUDRATE_230400  = 230400,  //!< 230400 bps.
    MP_UART_BAUDRATE_460800  = 460800,  //!< 460800 bps.
    MP_UART_BAUDRATE_921600  = 921600,  //!< 921600 bps.
    MP_UART_BAUDRATE_1843200 = 1843200, //!< 1843200 bps.
    MP_UART_BAUDRATE_3686400 = 3686400  //!< 3686400 bps.
}mp_uart_baudrate_t;

typedef enum
{
    MP_UART_DATA_5BITS = 5, //!< data of 5 bits.
    MP_UART_DATA_6BITS = 6, //!< data of 6 bits.
    MP_UART_DATA_7BITS = 7, //!< data of 7 bits.
    MP_UART_DATA_8BITS = 8, //!< data of 8 bits.
    MP_UART_DATA_9BITS = 9  //!< data of 9 bits.
}mp_uart_databits_t;

typedef enum
{
    MP_UART_PARITY_NO,      //!< No parity.
    MP_UART_PARITY_ODD,     //!< ODD parity.
    MP_UART_PARITY_EVEN     //!< Even parity.
}mp_uart_parity_t;

typedef enum
{
    MP_UART_STOPBIT_0_5,    //!< 0.5 stop bit.
    MP_UART_STOPBIT_1,      //!< 1 stop bit.
    MP_UART_STOPBIT_1_5,    //!< 1.5 stop bit.
    MP_UART_STOPBIT_2       //!< 2 stop bit.
}mp_uart_stopbit_t;

// Define ----------------------------------------------------------------------

// Structure -------------------------------------------------------------------
typedef struct
{
    mp_device_t device_parent;
} mp_uart_t;

// Include ---------------------------------------------------------------------
#include "mpDevicesTable.h"
#include "mp_uart_port.h"

// Extern global variables -----------------------------------------------------

// Public static 'virtual' functions -------------------------------------------

// Define all device macro to empty.
// Note: Only MP_DEV_UART will be redefine in this file for each functions.
#include "mp_def_empty_dev.h"

static inline int mp_uart_init(mp_device_id_t devid)
{
    int ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_init(devid);                      \
        }
    
    if(0){}
    MP_DEVICES_TABLE
        
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_deinit(mp_device_id_t devid)
{
    int ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_deinit(devid);                    \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_config(mp_device_id_t devid,
                                 mp_uart_baudrate_t baudrate,
                                 mp_uart_databits_t databit,
                                 mp_uart_parity_t parity,
                                 mp_uart_stopbit_t stopbit)
{
    int ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_config(devid, baudrate, databit,  \
                                            parity, stopbit);          \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline ssize_t mp_uart_write(mp_device_id_t devid,  void const * buf, size_t nbyte, mp_tick_t timeout)
{
    ssize_t ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_write(devid, buf, nbyte, timeout);\
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline ssize_t mp_uart_read(mp_device_id_t devid, void *buf, size_t nbyte, mp_tick_t timeout)
{
    ssize_t ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_read(devid, buf, nbyte, timeout); \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_waitEndTransmit(mp_device_id_t devid, mp_tick_t timeout)
{
    ssize_t ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_waitEndTransmit(devid, timeout);  \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_waitDataReceive(mp_device_id_t devid, mp_tick_t timeout)
{
    ssize_t ret = -1;
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_waitDataReceive(devid, timeout);  \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_ctl(mp_device_id_t devid, int request, ...)
{
    int ret = -1;
    va_list ap;
    va_start(ap, request);
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)                    \
        else if (MP_DEVICE_TYPE(devid) == MP_DRIVER_TYPE_UART_##driver)\
        {                                                              \
            ret = mp_uart_##driver##_ctl(devid, request, ap);          \
        }
    
    if(0){}
    MP_DEVICES_TABLE
    
    #undef MP_DEV_UART
    #define MP_DEV_UART(device, driver, peripheral)
    
    va_end(ap);
    return ret;
}

// Note: fonction pour fair des printf rapide. si plusiteur uarte son definie
// dans la table des device il peut étre préférable de re emplementer cette fonction.
// Todo: Completer/améliorer le comentére si dessus
// @note: le printf prend bocoup de place ...
#include <string.h>
static inline int mp_uart_printf(mp_device_id_t devid, char const * format, ...)
{
    int ret = -1;
    va_list ap;
    char tmpStr[128];
    
    va_start(ap, format);
    int n = vsnprintf(tmpStr, sizeof(tmpStr), format, ap);
    va_end(ap);
    ret = mp_uart_write(devid, tmpStr, n, MP_TICK_MAX);
    
    //ret = mp_uart_write(devid, format, strlen(format), MP_TICK_MAX);
    
    return ret;
}

#endif // MP_UART_H
