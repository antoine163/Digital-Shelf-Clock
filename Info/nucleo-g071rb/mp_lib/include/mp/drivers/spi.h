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

#ifndef MP_SPI_H
#define MP_SPI_H

// Include ---------------------------------------------------------------------
#include "mpDriversTable.h"
#include "mp_port_spi.h"

#include "mp_def_empty_drv.h"

// Include SPI drivers from devices, know by mpLib
//#include "..."

// Include additional SPI driver from device unknown by mpLib
#ifdef MP_ADD_SPI_DRIVER_HEADER
    #include MP_ADD_SPI_DRIVER_HEADER
#endif // MP_ADD_SPI_DRIVER_HEADER

// Typedef ---------------------------------------------------------------------
typedef void mp_spi_t;

// Extern global variables -----------------------------------------------------
#undef MP_DRV_SPI
#define MP_DRV_SPI(instance, driver, peripheral)                       \
    extern mp_spi_t *instance;
    
MP_DRIVER_TABLE
#undef MP_DRV_SPI
#define MP_DRV_SPI(instance, driver, peripheral)

// Static functions ---------------------------------------------------------
static inline int mp_spi_init(mp_spi_t *drv)
{
    int ret = -1;
    
    #undef MP_DRV_SPI
    #define MP_DRV_SPI(instance, driver, peripheral)                   \
        else if (drv == &instance)                                     \
        {                                                              \
            ret = mp_##driver##_spi_init(instance, peripheral);        \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_SPI
    #define MP_DRV_SPI(instance, driver, peripheral)
    
    return ret;
}

//int mp_spi_deinit(mp_adc_t *drv);
//int mp_spi_config(mp_adc_t *drv,  ...);


//int mp_spi_ctl(mp_adc_t *drv, int request, ...);

#endif // MP_UART_H

