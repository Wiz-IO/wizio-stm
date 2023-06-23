#include <stdio.h>
#include "main.h"
#include "stm32f3xx_it.h"

extern UART_HandleTypeDef huart1;

void NMI_Handler(void)
{
  while (1)
    continue;
}

void MemManage_Handler(void)
{
  while (1)
    continue;
}

void BusFault_Handler(void)
{
  while (1)
    continue;
}

void UsageFault_Handler(void)
{
  while (1)
    continue;
}

void DebugMon_Handler(void) {}

/* in os.c */
// void SVC_Handler(void) {}
// void PendSV_Handler(void) {}
// void SysTick_Handler(void) { HAL_IncTick(); }

#if 0

void HardFault_Handler(void)
{
  while (1)
    continue;
}

#else

/*    https://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html    */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void prvGetRegistersFromStack(uint32_t *p)
{
  volatile uint32_t r0;
  volatile uint32_t r1;
  volatile uint32_t r2;
  volatile uint32_t r3;
  volatile uint32_t r12;
  volatile uint32_t lr;  /* Link register. */
  volatile uint32_t pc;  /* Program counter. */
  volatile uint32_t psr; /* Program status register. */

  r0 = p[0];
  r1 = p[1];
  r2 = p[2];
  r3 = p[3];
  r12 = p[4];
  lr = p[5];
  pc = p[6];
  psr = p[7];

  printf("\n[ERROR] Hard Fault\n");
  printf(" - PSR = 0x%08X\n", (int)psr);
  printf(" - PC  = 0x%08X\n", (int)pc);
  printf(" - LR  = 0x%08X\n", (int)lr);

  for (;;)
    continue;
}
#pragma GCC diagnostic pop

void __attribute__((naked)) HardFault_Handler(void)
{
  __asm volatile(
      " tst lr, #4                                                \n"
      " ite eq                                                    \n"
      " mrseq r0, msp                                             \n"
      " mrsne r0, psp                                             \n"
      " ldr r1, [r0, #24]                                         \n"
      " ldr r2, handler2_address_const                            \n"
      " bx r2                                                     \n"
      " handler2_address_const: .word prvGetRegistersFromStack    \n");
}

#endif