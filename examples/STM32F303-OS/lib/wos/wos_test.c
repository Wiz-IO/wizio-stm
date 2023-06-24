/**
 ******************************************************************************
 *
 * Copyright (c) 2023 WizIO.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *   TEST BOARD: https://www.st.com/en/evaluation-tools/stm32f3discovery.html
 *
 ******************************************************************************
 */

#include <stdio.h>
#include "wos.h"

static void *handles[8];

void timer_callback(void *user)
{
    HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_8);
}

static void entry(void *params)
{

    int p = (int)params;
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOE, 1 << p);
        // wos_delay(100);
        HAL_Delay(100);
        wos_yield();
    }
}

void wos_test(void)
{
    wos_init(0);
    for (int i = 1; i < 8; i++)
        handles[i] = wos_task_create(&entry, (void *)(i + 8), NULL, WOS_CONFIG_MINIMAL_STACK_SIZE * 4);
    void *tmr = wos_timer_create(100, WOS_TIMER_PERIODIC, timer_callback, NULL);
    wos_timer_start(tmr);
    wos_start();
}