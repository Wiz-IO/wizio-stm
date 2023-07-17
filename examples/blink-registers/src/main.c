
/*
 ******************************************************************************
 *
 * Copyright (c) 2023 WizIO.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "stm32f3xx_hal.h"

void SystemInit() // from Reset_Handler()
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif
}

void _init(void) // from __libc_init_array()
{
    // OSC ... HSI-PLL 48 MHz
    RCC->CR |= RCC_CR_HSION;
    while (!(RCC->CR & RCC_CR_HSIRDY))
        continue;

    // POWER
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // FLASH
    FLASH->ACR = 0x12; // Prefetch enabled, Two wait

    RCC->CFGR = (4 << RCC_CFGR_PPRE1_Pos) |   // PPRE1  =  4 ( div  2 )
                (10 << RCC_CFGR_PLLMUL_Pos) | // PLLMUL = 10 ( mul 12 )
                RCC_CFGR_SW_PLL;              // PLL selected as system clock

    // PLL ENABLE for 48MHz
#if 1 // else 8 MHz
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY))
        continue;
#endif

    RCC->AHBENR |= 0b11111111 << 16;      // Clock Enable All GPIO
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Clock Enable
}

///////////////////////////////////////////////////////////////////////////////

typedef struct
{
    volatile uint32_t CSR;   // SysTick Control and Status Register
    volatile uint32_t RVR;   // SysTick Reload Value Register
    volatile uint32_t CVR;   // SysTick Current Value Register
    volatile uint32_t CALIB; // SysTick Calibration Value Register
} SysTick_t;
void sys_tick_delay(int delay)
{
    SysTick_t *pSysTick = ((SysTick_t *)0xE000E010);
    pSysTick->RVR = 48000;               // Sets the reload value
    pSysTick->CVR = 0;                   // Clears the SysTick current value register
    pSysTick->CSR = (1 << 0) | (1 << 2); // Enable SysTick in the control register and select internal system clock source
    for (int i = delay; i > 0; i--)
        while (!(pSysTick->CSR & (1 << 16)))
            continue;
    pSysTick->CSR = 0; // Disable SysTick counter in the Control Register
}

///////////////////////////////////////////////////////////////////////////////

#define UART USART1

inline __attribute__((always_inline)) void uart_transmit_char(char c)
{
    UART->TDR = c;
    while (0 == (UART->ISR & USART_ISR_TXE))
        continue;
}

void uart_transmit_string(char *txt)
{
    while (*txt)
        uart_transmit_char(*txt++);
}

void uart_init(void)
{
    GPIOC->AFR[0] |= (7 << GPIO_AFRL_AFRL4_Pos) | (7 << GPIO_AFRL_AFRL5_Pos); // alternate function
    GPIOC->MODER &= ~(0b111100000000);                                        // Clear 4 & 5
    GPIOC->MODER |= (GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1);              // Alternate Fxn mode
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5);                  // Push/Pull
    GPIOC->OSPEEDR |= (0b111100000000);                                       // High speed

    UART->CR1 = 0x00;              // Clear ALL
    UART->CR1 |= USART_CR1_UE;     // Enable USART
    UART->BRR = 48000000 / 115200; //
    // UART->CR1 &= ~USART_CR1_PCE;   // No parity control
    // UART->CR1 &= ~USART_CR2_STOP;  // 1 stop bit
    // UART->CR1 &= ~USART_CR3_CTSE;  // No hardware flow control
    // UART->CR1 &= ~USART_CR3_RTSE;  // No hardware flow control
    UART->CR1 |= USART_CR1_RE; // Recieve enable
    UART->CR1 |= USART_CR1_TE; // Transmit enable
}

///////////////////////////////////////////////////////////////////////////////

#define LED_HI() GPIOE->BSRR |= GPIO_PIN_8;
#define LED_LO() GPIOE->BSRR |= GPIO_PIN_8 << 16

void led_init()
{
    GPIOE->MODER &= ~(GPIO_MODER_MODER8);                // 0: Input mode (reset state)
    GPIOE->MODER |= GPIO_MODER_MODER8_0;                 // 1: General purpose output mode
    GPIOE->OTYPER &= ~(GPIO_OTYPER_OT_8);                // 0: Output push-pull (reset state)
    GPIOE->OSPEEDR |= (3 << GPIO_OSPEEDER_OSPEEDR8_Pos); // 3: High speed
}

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
    led_init();
    uart_init();
    uart_transmit_string("\nSETUP\n");
    while (1)
    {
        LED_HI();
        sys_tick_delay(100);
        LED_LO();
        sys_tick_delay(100);
        uart_transmit_string("LOOP ");
    }
}
