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
 * #define MP_DEVICES_TABLE                                             \
 *     MP_DEV_ADC(dev_adc1,    port,       ADC1)                        \
 *     MP_DEV_SPI(dev_spi1,    port,       SPI1)                        \
 *     MP_DEV_ADC(dev_adc2,    ads7822,    drv_spi1)
 * @endcode
 * 
 * The result give :
 * @code
 * static mp_port_spi_t _dev_spi1;
 * mp_spi_t * const dev_spi1 = (mp_spi_t*)&_dev_spi1;
 *
 * static mp_port_adc_t _dev_adc1;
 * mp_adc_t * const dev_adc1 = (mp_adc_t*)&_dev_adc1;
 *
 * static mp_ads7822_adc_t _dev_adc2;
 * mp_adc_t * const dev_adc2 = (mp_adc_t*)&_dev_adc2;
 * @endcode
 */
 
// Include ---------------------------------------------------------------------
#include "mp.h"
#include "mpDevicesTable.h"

// Global variables ------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Reserve memory for all devices
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Undef all macro defined device 
#include "mp_undef_all_dev.h"

// Define all device macro to build instances
#define MP_DEV_GPIO(device, driver, peripheral)                        \
    mp_gpio_##driver##_t _##device =                                   \
    {                                                                  \
        .gpio_parent.device_parent.devid = device,                     \
        .gpiox = peripheral,                                           \
    };

#define MP_DEV_UART(device, driver, peripheral)                        \
    mp_uart_##driver##_t _##device =                                   \
    {                                                                  \
        .uart_parent.device_parent.devid = device,                     \
        .uartx = peripheral,                                           \
    };

// Instantiate all devices
MP_DEVICES_TABLE

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Build the devices table
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Undef all macro defined device 
#include "mp_undef_all_dev.h"

// Build table with all device pointer
#define MP_DEV_GPIO(device, driver, peripheral)                        \
    (mp_device_t * const)&_##device,
#define MP_DEV_UART(device, driver, peripheral)                        \
    (mp_device_t * const)&_##device,

mp_device_t * const _mp_devices_table[] = { MP_DEVICES_TABLE };
