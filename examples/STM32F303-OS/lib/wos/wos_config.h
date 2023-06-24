#ifndef WOS_CONFIG_H
#define WOS_CONFIG_H

// clang-format off

#include "stm32f3xx_hal.h"

// #define WOS_CONFIG_PREEMPTION

// #define WOS_CONFIG_SWITCH_TIME        (10) /* ms, default = systick */

#define WOS_CONFIG_MINIMAL_STACK_SIZE   (128) /* Words */

#define WOS_CONFIG_TIMERS

// clang-format on

#endif /* WOS_CONFIG_H */
