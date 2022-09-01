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
  
#ifndef MP_DRIVERS_H
#define MP_DRIVERS_H

// Include ---------------------------------------------------------------------
#include "mpDevicesTable.h"

// Include the file where are defined additional drivers unknown by mpLib
#ifdef MP_ADD_DRIVER_HEADER
    #include MP_ADD_DRIVER_HEADER
#endif // MP_EX_DRIVER_HEADER

// Define ----------------------------------------------------------------------
// MP_DRIVER(<type>_<driver>)
// - <type>: uppercase
// - <driver>: lowercase
// eg: MP_DRIVER(GPIO_port)
#define MP_DRIVER_TABLE                                                \
    MP_DRIVER(GPIO, port)                                              \
    MP_DRIVER(UART, port)

// enum ------------------------------------------------------------------------

// Build drivers type id
typedef enum
{
    #define MP_DRIVER(type, driver)                                    \
            MP_DRIVER_TYPE_##type##_##driver,
    
    // Extend drivers table with additional drivers unknown by mpLib
    #ifdef MP_ADD_DRIVERS_TABLE
        MP_ADD_DRIVERS_TABLE
    #endif // MP_ADD_DRIVERS_TABLE

    MP_DRIVER_TABLE
}mp_driver_type_t;

#endif // MP_DRIVERS_H

