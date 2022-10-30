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

// Includes --------------------------------------------------------------------
// App
#include "board/board.h"
#include "tasks/task_test.h"

// Driver from mpLib
#include "mp/drivers/interrupt.h"
#include "mp/drivers/tick.h"
#include "mp/drivers/gpio.h"

// FreeRtos
#include "FreeRTOS.h"
#include "task.h"

// Implemented functions -------------------------------------------------------
/* Structure that will hold the TCB of the task being created. */
    StaticTask_t xTaskBuffer_test1;
    StaticTask_t xTaskBuffer_test2;

    /* Buffer that the task being created will use as its stack.  Note this is
    an array of StackType_t variables.  The size of StackType_t is dependent on
    the RTOS port. */
    StackType_t xStack_test1[ configMINIMAL_STACK_SIZE ];
    StackType_t xStack_test2[ configMINIMAL_STACK_SIZE ];
    
int main()
{
    systemInit();
    
    mp_interrupt_init();
    mp_tick_init();
    
    
    
    // Init gpios
    mp_gpio_init(dev_gpioa);
    mp_gpio_init(dev_gpiob);
    mp_gpio_init(dev_gpioc);
    mp_gpio_init(dev_gpiod);
    mp_gpio_init(dev_gpiof);
    
    /* GPIO OUT:    name,           type,   pull,   default level             */
    MP_GPIO_OUT(LED_GREEN,      PUSH_PULL,  NO,     0);
    MP_GPIO_OUT(PIN_LED_RED,    PUSH_PULL,  NO,     1);
    MP_GPIO_OUT(PIN_LED_GREEN,  PUSH_PULL,  NO,     LOW);
    MP_GPIO_OUT(PIN_LED_YELLOW, PUSH_PULL,  NO,     LOW);
    /* AFF_7SEG                                                               */
    MP_GPIO_OUT(AFF_7SEG,       PUSH_PULL,  NO,     MASK(0x7f));
    /* GPIO IN:     name,                   pull                              */
    MP_GPIO_IN( BP1,                        NO);
    MP_GPIO_IN( BP2,                        UP);
    MP_GPIO_IN( PIN_BP_LED,                 UP);

    // grep "<mp[^ ]*>:" digital_shelf_clock.lss
    // arm-none-eabi-readelf digital_shelf_clock.elf -wi > /tmp/readelf
    //mp_gpio_enableIsr(BP1, MP_GPIO_TRIGGER_FALLING, bp1Handler);
    //mp_gpio_enableIsr(BP2, MP_GPIO_TRIGGER_FALLING, bp2Handler);
    //mp_gpio_enableIsr(PIN_BP_LED, MP_GPIO_TRIGGER_RISING, bpLedHandler);
    
    
    
    
    /* Create the task without using any dynamic memory allocation. */
     xTaskCreateStatic(
                  task_test1,       /* Function that implements the task. */
                  "Test1",          /* Text name for the task. */
                  sizeof(xStack_test1)/sizeof(StackType_t),      /* Number of indexes in the xStack array. */
                  ( void * ) NULL,    /* Parameter passed into the task. */
                  tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                  xStack_test1,          /* Array to use as the task's stack. */
                  &xTaskBuffer_test1 );  /* Variable to hold the task's data structure. */
    
    ///* Create the task without using any dynamic memory allocation. */
     xTaskCreateStatic(
                  task_test2,       /* Function that implements the task. */
                  "Test2",          /* Text name for the task. */
                  sizeof(xStack_test2)/sizeof(StackType_t),      /* Number of indexes in the xStack array. */
                  ( void * ) NULL,    /* Parameter passed into the task. */
                  tskIDLE_PRIORITY+1,/* Priority at which the task is created. */
                  xStack_test2,          /* Array to use as the task's stack. */
                  &xTaskBuffer_test2 );  /* Variable to hold the task's data structure. */

    vTaskStartScheduler();

    return 0;
}

void HardFault_Handler()
{
    asm("BKPT 0");
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
