#include "main.h"
#include "stm32f1xx_it.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

void NMI_Handler(void)
{
  while (1)
  {
  }
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void DebugMon_Handler(void)
{
}

#ifndef FREERTOS
void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}
#endif



void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart3);
}

