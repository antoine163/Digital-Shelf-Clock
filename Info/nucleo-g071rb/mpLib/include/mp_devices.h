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

// FreeRtos
#if defined MP_USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

// Define macro ----------------------------------------------------------------
#define MP_DEVICE(dev)           ((mp_device_t *)dev)
#define MP_DEVICE_GET(devid)     mp_device_get(devid)

#define MP_DEVICE_ID(devid)         ( devid & 0x0000ffff )
#define MP_DEVICE_TYPE(devid)     ( ( devid & 0xffff0000 ) >> 16 )

#if defined __DOXYGEN__
    /**
     * @brief Macro to send synchro from ISR.
     * 
     * - If @ref MP_USE_FREERTOS is defined, this macro send a Task Notifications
     * with @b vTaskNotifyGiveIndexedFromISR() function. The follow parameters 
     * as send to this function (device is get from dev):
     *  - @p xTaskHandle : device->taskToNotify, the task where the Notifications
     * will send. It is initialized with the handler which initialized the device.
     *  - @p uxIndexToNotify : MP_DEVICE_ID(device->devid), the identification of
     * device is use like index of Notifications.
     *  - @p pxHigherPriorityTaskWoken : device->higherPriorityTaskWoken
     * 
     * - If @ref MP_USE_FREERTOS is not defined, this macro is empty.
     * 
     * @param dev Points to an object inherited from @ref mp_device_t.
     * @param sync_type Integer value to identify the synchro. The synchro will
     * be send only if the call code ask this, from @reg MP_DEVICE_WAIT_SYNC().
     * @b Note: It no used if @ref MP_USE_FREERTOS is defined.
     * 
     * @see MP_DEVICE_WAIT_SYNC()
     */
    #define MP_DEVICE_SEND_SYNC_FROM_ISR(dev, sync_type)
    
    /**
     * @brief Macro to wait synchro.
     * 
     * - If @ref MP_USE_FREERTOS is defined, this macro wait @p code_condition
     * and Task Notifications with @b ulTaskNotifyTakeIndexed() function.
     * The follow parameters as send to this function (device is get from dev):
     *  - uxIndexToWaitOn : MP_DEVICE_ID(device->devid), the identification of
     * device is use like index of Notifications.
     *  - xClearCountOnExit : pdTRUE
     *  - xTicksToWait : @p timeout
     * 
     * - If @ref MP_USE_FREERTOS is not defined, this macro wait
     * @p code_condition or @p timeout.
     * 
     * @param dev Points to an object inherited from @ref mp_device_t.
     * @param code_condition A code condition to check the synchro.
     * @param sync_type Integer value to identify the synchro. @b Note: It no used
     * if @ref MP_USE_FREERTOS is defined.
     * @param timeout The timeout in tick from @ref mp_tick_t.
     * @param code_out A code to execute if timeout occur. For example:
     * - If you want exit the current function, you can write for this parameter
     * 'return -1'.
     * - If you don't want exit the current function, but you want to know if
     * the timeout as occurred, you can write for this parameter 'n=-1'
     * 
     * @see MP_DEVICE_SEND_SYNC_FROM_ISR()
     */
    #define MP_DEVICE_WAIT_SYNC(dev,    code_condition, sync_type,     \
                                        timeout,        code_out) 

#elif defined MP_USE_FREERTOS
    // MP_DEVICE_SEND_SYNC_FROM_ISR defined for FreeRtos
    #define MP_DEVICE_SEND_SYNC_FROM_ISR(dev, sync_type)               \
    do {                                                               \
        mp_device_t * device = MP_DEVICE(dev);                         \
        /* Send notify only if the task ask for it */                  \
        if (device->syncType & sync_type)                              \
        {                                                              \
            /* Clean synchro type  to don't send synchro for nothing */\
            device->syncType = 0;                                      \
                                                                       \
            /* Send notify to unblock task waiting */                  \
            vTaskNotifyGiveIndexedFromISR(                             \
                device->taskToNotify, MP_DEVICE_ID(device->devid),     \
                &device->higherPriorityTaskWoken);                     \
        }                                                              \
    } while (0)

    // MP_DEVICE_WAIT_SYNC defined for FreeRtos
    #define MP_DEVICE_WAIT_SYNC(dev,    code_condition, sync_type,     \
                                        timeout,        code_out)      \
    do {                                                               \
        TimeOut_t tickStart;                                           \
        mp_device_t * device = MP_DEVICE(dev);                         \
                                                                       \
        vTaskSetTimeOutState(&tickStart);                              \
        device->syncType = sync_type;                                  \
                                                                       \
        while ( !(code_condition) )                                    \
        {                                                              \
            if (xTaskCheckForTimeOut(&tickStart, &timeout) == pdTRUE)  \
            {                                                          \
                device->syncType = 0;                                  \
                code_out;                                              \
                break;                                                 \
            }                                                          \
                                                                       \
            ulTaskNotifyTakeIndexed(                                   \
                MP_DEVICE_ID(device->devid),                           \
                pdTRUE, timeout);                                      \
        }                                                              \
    } while (0);
#else
    // MP_DEVICE_SEND_SYNC_FROM_ISR defined for baremetal
    #define MP_DEVICE_SEND_SYNC_FROM_ISR(dev, sync_type) do {} while (0)

    // MP_DEVICE_WAIT_SYNC defined for baremetal
    #define MP_DEVICE_WAIT_SYNC(dev,    code_condition, sync_type,     \
                                        timeout,        code_out)      \
    do {                                                               \
        mp_tick_t tickStart = mp_tick_get();                           \
                                                                       \
        while ( !(code_condition) )                                    \
        {                                                              \
            if (mp_tick_get() - tickStart >= timeout)                  \
            {                                                          \
                code_out;                                              \
                break;                                                 \
            }                                                          \
                                                                       \
            __WFI();                                                   \
        }                                                              \
    } while (0)
#endif

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
    
#if defined MP_USE_FREERTOS
    TaskHandle_t taskToNotify;
    BaseType_t higherPriorityTaskWoken;
    uint32_t syncType;
#endif
} mp_device_t;

// extern global variable ------------------------------------------------------
extern mp_device_t * const _mp_devices_table[];

// Static inline function ------------------------------------------------------
static inline mp_device_t * mp_device_get(mp_device_id_t devid)
{
    return _mp_devices_table[MP_DEVICE_ID(devid)];
}

#endif // MP_DEVICES_H

