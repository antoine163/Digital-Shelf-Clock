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

#ifndef MP_DRIVER_UART_H
#define MP_DRIVER_UART_H

// Include ---------------------------------------------------------------------
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

// Typedef ---------------------------------------------------------------------
typedef void mp_uart_t;

// Enum ------------------------------------------------------------------------
typedef enum
{
    MP_UART_BAUDRATE_300    = 600,      //!< 600 bps.
    MP_UART_BAUDRATE_1200   = 1200,     //!< 1200 bps.
    MP_UART_BAUDRATE_2400   = 2400,     //!< 2400 bps.
    MP_UART_BAUDRATE_4800   = 4800,     //!< 4800 bps.
    MP_UART_BAUDRATE_9600   = 9600,     //!< 9600 bps.
    MP_UART_BAUDRATE_19200  = 19200,    //!< 19200 bps.
    MP_UART_BAUDRATE_38400  = 38400,    //!< 38400 bps.
    MP_UART_BAUDRATE_57600  = 57600,    //!< 57600 bps.
    MP_UART_BAUDRATE_115200 = 115200,   //!< 115200 bps.
    MP_UART_BAUDRATE_230400 = 230400    //!< 230400 bps.
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
    MP_UART_PARITY_NO   = 0,    //!< No parity.
    MP_UART_PARITY_ODD  = 1,    //!< ODD parity.
    MP_UART_PARITY_EVEN = 2     //!< Even parity.
}mp_uart_parity_t;

typedef enum
{
    MP_UART_STOPBIT_0_5 = 0,    //!< 0.5 stop bit.
    MP_UART_STOPBIT_1   = 1,    //!< 1 stop bit.
    MP_UART_STOPBIT_1_5 = 2,    //!< 1.5 stop bit.
    MP_UART_STOPBIT_2   = 3     //!< 2 stop bit.
}mp_uart_stopbit_t;

// Include ---------------------------------------------------------------------
#include "mpDevicesTable.h"
#include "mp_port_uart.h"

#include "mp_def_empty_drv.h"

// Include UART drivers from devices, know by mpLib
//#include "..."

// Include additional UART driver from device unknown by mpLib
#ifdef MP_ADD_UART_DRIVER_HEADER
    #include MP_ADD_UART_DRIVER_HEADER
#endif // MP_ADD_UART_DRIVER_HEADER

// Extern global variables -----------------------------------------------------
#undef MP_DRV_UART
#define MP_DRV_UART(instance, driver, peripheral)                      \
    extern mp_uart_t * instance;
    
MP_DRIVER_TABLE
#undef MP_DRV_UART
#define MP_DRV_UART(instance, driver, peripheral)

// Static functions ---------------------------------------------------------
static inline int mp_uart_init(mp_uart_t * drv)
{
    int ret = -1;
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)                  \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_uart_init(instance, peripheral);       \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_deinit(mp_uart_t *drv)
{
    int ret = -1;
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)                  \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_uart_deinit(instance);                 \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_config(mp_uart_t *drv,
                                 mp_uart_baudrate_t baudrate,
                                 mp_uart_databits_t databit,
                                 mp_uart_parity_t parity,
                                 mp_uart_stopbit_t stopbit)
{
    int ret = -1;
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)                  \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_uart_config(instance, baudrate, databit,  \
                                        parity, stopbit);              \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_write(mp_uart_t *drv, const void *buf, size_t nbyte)
{
    int ret = -1;
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)                  \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_uart_write(instance, buf, nbyte);        \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_read(mp_uart_t *drv, void *buf, size_t nbyte)
{
    int ret = -1;
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)                  \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_uart_read(instance, buf, nbyte);       \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)
    
    return ret;
}

static inline int mp_uart_ctl(mp_uart_t *drv, int request, ...)
{
    int ret = -1;
    va_list ap;
    va_start(ap, request);
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)                  \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_uart_ctl(instance, request, ap);       \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_UART
    #define MP_DRV_UART(instance, driver, peripheral)
    
    va_end(ap);
    return ret;
}


// A supprimé
static inline int mp_uart_printf(mp_uart_t *drv, const char *format, ...)
{
    int ret = -1;
    va_list ap;
    char tmpStr[32];
    
    va_start(ap, format);
    //int n = vsnprintf(tmpStr, sizeof(tmpStr), format, ap);
    int n = vsnprintf(tmpStr, sizeof(tmpStr), format, ap);
    va_end(ap);
    ret = mp_uart_write(drv, tmpStr, n);
    
    return ret;
}


#endif // MP_DRIVER_UART_H
