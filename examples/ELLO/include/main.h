#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <reent.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "stm32g0xx_hal.h"

  void Error_Handler(void);
  void board_config(void);

#define MBCS_Pin GPIO_PIN_15
#define MBCS_GPIO_Port GPIOC
#define MBAN_Pin GPIO_PIN_0
#define MBAN_GPIO_Port GPIOA
#define VSYNC_Pin GPIO_PIN_1
#define VSYNC_GPIO_Port GPIOA
#define BLUE_Pin GPIO_PIN_2
#define BLUE_GPIO_Port GPIOA
#define HSYNC_Pin GPIO_PIN_4
#define HSYNC_GPIO_Port GPIOA
#define RED_Pin GPIO_PIN_10
#define RED_GPIO_Port GPIOA
#define GREEN_Pin GPIO_PIN_5
#define GREEN_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
