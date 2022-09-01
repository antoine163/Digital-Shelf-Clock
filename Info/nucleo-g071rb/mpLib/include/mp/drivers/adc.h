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

#ifndef MP_ADC_H
#define MP_ADC_H

// Typedef ---------------------------------------------------------------------
typedef void mp_adc_t;

// Include ---------------------------------------------------------------------
#include "mpDriversTable.h"
#include "mp_port_adc.h"

// Include ADC drivers from devices, know by mpLib
#include "mp/devices/adc_ads7822.h"

// Include additional ADC driver from device unknown by mpLib
#ifdef MP_ADD_ADC_DRIVER_HEADER
    #include MP_ADD_ADC_DRIVER_HEADER
#endif // MP_ADD_ADC_DRIVER_HEADER

#include "mp_def_empty_drv.h"

// Extern global variables -----------------------------------------------------
#undef MP_DRV_ADC
#define MP_DRV_ADC(instance, driver, peripheral)                       \
    extern mp_adc_t *instance;
    
MP_DRIVER_TABLE
#undef MP_DRV_ADC
#define MP_DRV_ADC(instance, driver, peripheral)


// Static functions ---------------------------------------------------------
static inline int mp_adc_init(mp_adc_t *drv)
{
    int ret = -1;
    
    #undef MP_DRV_ADC
    #define MP_DRV_ADC(instance, driver, peripheral)                   \
        else if (drv == instance)                                      \
        {                                                              \
            ret = mp_##driver##_adc_init(instance, peripheral);        \
        }
    
    if(0){}
    MP_DRIVER_TABLE
    
    #undef MP_DRV_ADC
    #define MP_DRV_ADC(instance, driver, peripheral)
    
    //if(0){}
    //else if(drv == drv_adc1)
    //{
        //mp_port_adc_init(_drv_adc1, ADC1);
    //}
        
    return ret;
}

//int mp_adc_deinit(mp_adc_t *drv);
//int mp_adc_config(mp_adc_t *drv,  ...);

//int mp_adc_convert(mp_adc_t *drv, unsigned int channel);

//int mp_adc_ctl(mp_adc_t *drv, int request, ...);

//static inline int mp_adc_get_volatag(mp_adc_t *drv, unsigned int channel)
//{
    //int adcVal = mp_adc_convert(drv, channel);
    //return (adcVal * drv->vref) / drv->res;
//}

#endif // MP_UART_H
