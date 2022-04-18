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

// Includes --------------------------------------------------------------------

// Implemented functions -------------------------------------------------------

void testHandler()
{   
    int a=0;
    while(1)
    {
        a++;
    }
}

//void PendSV_Handler()
//{
    //xPortPendSVHandler();
//}

//void SysTick_Handler()
//{
    //xPortSysTickHandler();
//}


//void SVC_Handler()
//{
    //vPortSVCHandler();
//}


void NMI_Handler() {testHandler();}
void HardFault_Handler() {testHandler();}
void WWDG_IRQHandler() {testHandler();}
void PVD_IRQHandler() {testHandler();}
void RTC_TAMP_IRQHandler() {testHandler();}
void FLASH_IRQHandler() {testHandler();}
void RCC_IRQHandler() {testHandler();}
void EXTI0_1_IRQHandler() {testHandler();}
void EXTI2_3_IRQHandler() {testHandler();}
void EXTI4_15_IRQHandler() {testHandler();}
void UCPD1_2_IRQHandler() {testHandler();}
void DMA1_Channel1_IRQHandler() {testHandler();}
void DMA1_Channel2_3_IRQHandler() {testHandler();}
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler() {testHandler();}
void ADC1_COMP_IRQHandler() {testHandler();}
void TIM1_BRK_UP_TRG_COM_IRQHandler() {testHandler();}
void TIM1_CC_IRQHandler() {testHandler();}
void TIM2_IRQHandler() {testHandler();}
void TIM3_IRQHandler() {testHandler();}
void TIM6_DAC_LPTIM1_IRQHandler() {testHandler();}
void TIM7_LPTIM2_IRQHandler() {testHandler();}
void TIM14_IRQHandler() {testHandler();}
void TIM15_IRQHandler() {testHandler();}
void TIM16_IRQHandler() {testHandler();}
void TIM17_IRQHandler() {testHandler();}
void I2C1_IRQHandler() {testHandler();}
void I2C2_IRQHandler() {testHandler();}
void SPI1_IRQHandler() {testHandler();}
void SPI2_IRQHandler() {testHandler();}
void USART1_IRQHandler() {testHandler();}
//void USART2_IRQHandler() {testHandler();}
void USART3_4_LPUART1_IRQHandler() {testHandler();}
void CEC_IRQHandler() {testHandler();}
