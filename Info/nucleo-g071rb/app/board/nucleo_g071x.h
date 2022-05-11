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
 
#ifndef NUCLEO_G071X_H
#define NUCLEO_G071X_H

// Include ---------------------------------------------------------------------
#include "stm32g0xx_ll_gpio.h"

// Defines ---------------------------------------------------------------------
//#define LED_GREEN_Port      GPIOA
//#define LED_GREEN_Pin       LL_GPIO_PIN_5
//#define BP1_Port            GPIOC
//#define BP1_Pin             LL_GPIO_PIN_13
//#define BP2_Port            GPIOB
//#define BP2_Pin             LL_GPIO_PIN_7

// Static functions ------------------------------------------------------------
//static inline void boardLedGreenToggle()
//{
    //LL_GPIO_TogglePin(LED_GREEN_Port, LED_GREEN_Pin);
//}

//static inline void boardLedGreenOn()
//{
    //LL_GPIO_SetOutputPin(LED_GREEN_Port, LED_GREEN_Pin);
//}

//static inline void boardLedGreenOff()
//{
    //LL_GPIO_ResetOutputPin(LED_GREEN_Port, LED_GREEN_Pin);
//}

#endif // NUCLEO_G071X_H
