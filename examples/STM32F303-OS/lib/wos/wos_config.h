#ifndef OS_CONFIG_H
#define OS_CONFIG_H
// clang-format off

#include "stm32f3xx_hal.h"

// #define OS_CONFIG_PREEMPTION

// #define OS_CONFIG_SWITCH_TIME        (10) /* ms, default = systick */

#define OS_CONFIG_MINIMAL_STACK_SIZE    (128) /* Words */

#define OS_CONFIG_TIMERS

// clang-format on
#endif /* OS_CONFIG_H */
