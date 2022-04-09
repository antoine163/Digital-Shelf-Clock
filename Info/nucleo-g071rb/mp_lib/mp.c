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
 
 /**
 * @file This file has role to instantiate in global all drivers structure.
 * 
 * For example, for a * MP_DRIVER_TABLE below :
 * @code
 * #define MP_DRIVER_TABLE                                              \
 *     MP_DRV_ADC(drv_adc1,    port,       ADC1)                        \
 *     MP_DRV_SPI(drv_spi1,    port,       SPI1)                        \
 *     MP_DRV_ADC(drv_adc2,    ads7822,    drv_spi1)
 * @endcode
 * 
 * The result give :
 * @code
 * static mp_port_spi_t _drv_spi1;
 * mp_spi_t *drv_spi1 = (mp_spi_t*)&_drv_spi1;
 *
 * static mp_port_adc_t _drv_adc1;
 * mp_adc_t *drv_adc1 = (mp_adc_t*)&_drv_adc1;
 *
 * static mp_ads7822_adc_t _drv_adc2;
 * mp_adc_t *drv_adc2 = (mp_adc_t*)&_drv_adc2;
 * @endcode
 */
 
// Include ---------------------------------------------------------------------
#include "mp.h"
#include "mpDriversTable.h"

// Global variables ------------------------------------------------------------
#include "mp_def_empty_drv.h"

#undef MP_DRV_UART
#define MP_DRV_UART(instance, driver, peripheral)                      \
    static mp_##driver##_uart_t _##instance;                           \
    mp_uart_t *instance = (mp_uart_t*)&_##instance;

#undef MP_DRV_ADC
#define MP_DRV_ADC(instance, driver, peripheral)                       \
    static mp_##driver##_adc_t _##instance;                            \
    mp_adc_t *instance = (mp_adc_t*)&_##instance;

#undef MP_DRV_SPI
#define MP_DRV_SPI(instance, driver, peripheral)                       \
    static mp_##driver##_spi_t _##instance;                            \
    mp_spi_t *instance = (mp_spi_t*)&_##instance;
    
MP_DRIVER_TABLE

#include "mp_def_empty_drv.h"


