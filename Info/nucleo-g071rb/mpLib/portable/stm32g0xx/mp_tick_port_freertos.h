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

#ifndef MP_TICK_PORT_FREERTOS_H
#define MP_TICK_PORT_FREERTOS_H

// For Freertos add
// #define xPortSysTickHandler         mp_uart_port_systick_isr
// In FreeRTOSConfig.h file

// Include ---------------------------------------------------------------------
#include "FreeRTOS.h"
#include "task.h"

// Define macro ----------------------------------------------------------------
#define MP_TICK_PORT_MAX            portMAX_DELAY
#define MP_TICK_PORT_FROM_MS(ms)    ( ms / portTICK_PERIOD_MS )
#define MP_TICK_PORT_TO_MS(tick)    ( tick * portTICK_PERIOD_MS )

// Typedef ---------------------------------------------------------------------
typedef TickType_t mp_tick_t;

// Structure -------------------------------------------------------------------

// Extern protected global variables -------------------------------------------

// Prototype functions ---------------------------------------------------------

// Static inline functions -----------------------------------------------------
static inline int mp_tick_port_init()
{
    return 0;
}

static inline int mp_tick_port_deinit()
{
    return 0;
}

static inline mp_tick_t mp_tick_port_get()
{
    return xTaskGetTickCount();
}

static inline mp_tick_t mp_tick_port_getFromIsr()
{
    return xTaskGetTickCountFromISR();
}

static inline void mp_tick_port_delay(mp_tick_t delay)
{
    vTaskDelay(delay);
}



#endif // MP_TICK_PORT_FREERTOS_H
