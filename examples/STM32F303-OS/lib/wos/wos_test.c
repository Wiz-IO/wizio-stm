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
        //wos_delay(100);
        HAL_Delay(100);
        wos_yield();
    }
}

void ms_cb(void)
{
    for (int i = 8; i < 16; i++)
    {
        HAL_GPIO_TogglePin(GPIOE, 1 << i);
        HAL_Delay(100);
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