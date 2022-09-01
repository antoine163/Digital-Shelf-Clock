/**
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
 
#ifndef MP_DEVICES_TABLE_H
#define MP_DEVICES_TABLE_H

/**
 * @todo a revoire
 * @brief table of devices used in application
 *
 * @p device the name of instance driver
 *
 * @p driver_name pressie 'port' to use the driver of the port. Or pressie other name
 * to use a specified driver
 *
 * @p peripheral pointer to a peripheral to use, NULL if not defined here.
 *
 * MP_DRV_UART(device, driver, peripheral)
 * MP_DRV_ADC( device, driver, peripheral)
 * MP_DRV_SPI( device, driver, peripheral)
 */
#define MP_DEVICES_TABLE                                               \
    MP_DEV_GPIO(dev_gpioa,      port,       GPIOA)                     \
    MP_DEV_GPIO(dev_gpiob,      port,       GPIOB)                     \
    MP_DEV_GPIO(dev_gpioc,      port,       GPIOC)                     \
    MP_DEV_GPIO(dev_gpiod,      port,       GPIOD)                     \
    MP_DEV_GPIO(dev_gpiof,      port,       GPIOF)                     \
    MP_DEV_UART(dev_ws2812b,    port,       USART1)                    \
    MP_DEV_UART(dev_tty,        port,       USART2)
 
 
#if 0
#define MP_DEVICES_TABLE                                               \
    MP_DEV_UART(drv_tty,    port,       USART2)                        \
    MP_DEV_SPI( dev_spi1,   port,       SPI1)                          \
    MP_DEV_ADC( dev_adc1,   port,       ADC1)                          \
    MP_DEV_ADC( dev_adc2,   ads7822,    drv_spi1)                      \
    MP_DEV_GPIO(dev_gpioa,  port,       GPIOA)                         \
    MP_DEV_GPIO(dev_gpiob,  port,       GPIOB)                         \
    MP_DEV_GPIO(dev_gpioc,  port,       GPIOC)                         \
    MP_DEV_GPIO(dev_gpiod,  port,       GPIOD)                         \
    MP_DEV_GPIO(dev_gpiof,  port,       GPIOF)
    //MP_DRV_GPIO(drv_gpioex1, gpioex,     drv_spi1, GPIOEX1_IRQ) // le driver gpioEx via récupérer l'interruption externe (le numéro de pin et le port) via le mpHardMap.h ...
    //MP_DRV_GPIO(drv_gpioex2, gpioex,     drv_spi1, GPIOEX2_IRQ) // le driver gpioEx via récupérer l'interruption externe (le numéro de pin et le port) via le mpHardMap.h ...
    //MP_DRV_EXTI(    drv_exti,   port,  LL_EXTI_LINE_7)
#endif
    
#endif // MP_DEVICES_TABLE_H
