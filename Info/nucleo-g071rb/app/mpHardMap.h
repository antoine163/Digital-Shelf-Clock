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
 
#ifndef MP_HARD_MAP_H
#define MP_HARD_MAP_H

// Defines ---------------------------------------------------------------------
#define PIN_LED_RED         MP_GPIO_PORT(drv_gpio, B, 13)
#define PIN_LED_GREEN       MP_GPIO_PORT(drv_gpio, B, 10)
#define PIN_LED_YELLOW      MP_GPIO_PORT(drv_gpio, B, 15)

#define PIN_BP_LED          MP_GPIO_PORT(drv_gpio, B, 2)


#define LED_GREEN       MP_GPIO_PORT(drv_gpio, A, 5)
#define BP1             MP_GPIO_PORT(drv_gpio, C, 13)
#define BP2             MP_GPIO_PORT(drv_gpio, B, 7)

//Group led green, red and bleu
#define LED_VECTOR      MP_GPIO(drv_gpio, A,    MP_GPIO_PORT_PIN(5) |  \
                                                MP_GPIO_PORT_PIN(6) |  \
                                                MP_GPIO_PORT_PIN(7))

/**
 * @brief Table of GPIO
 * 
 * type :
 * - PUSH_PULL (@ref MP_GPIO_TYPE_PUSH_PULL)
 * - OPEN_DRAIN (@ref MP_GPIO_TYPE_OPEN_DRAIN)
 * - OPEN_SOURCE (@ref MP_GPIO_TYPE_OPEN_SOURCE)
 * 
 * pull :
 * - NO (@ref MP_GPIO_PULL_NO)
 * - PULL_UP (@ref MP_GPIO_PULL_UP)
 * - PULL_DOWN (@ref MP_GPIO_PULL_DOWN)
 * 
 */
#define MP_GPIO_TABLE                                                  \
/* GPIO OUT:    name,           type,       pull,   default value    */\
    MP_GPIO_OUT(LED_GREEN,      PUSH_PULL,  NO,     1)                 \
    MP_GPIO_OUT(PIN_LED_RED,    PUSH_PULL,  NO,     1)                 \
    MP_GPIO_OUT(PIN_LED_GREEN,  PUSH_PULL,  NO,     1)                 \
    MP_GPIO_OUT(PIN_LED_YELLOW, PUSH_PULL,  NO,     1)                 \
/* GPIO IN:     name,                       pull                     */\
    MP_GPIO_IN( BP1,                        NO)                        \
    MP_GPIO_IN( BP2,                        UP)                        \
    MP_GPIO_IN( PIN_BP_LED,                 UP)
    
    

#endif // MP_HARD_MAP_H
