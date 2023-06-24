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
 ******************************************************************************
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "wos_config.h"

int wos_init(void *);
void wos_start(void);
void wos_suspend(void);
void wos_resume(void);
void wos_delay(uint32_t ms);
static inline void wos_yield(void) { asm volatile("svc #0"); }

void *wos_task_create(void (*entry)(void *params), void *params, uint32_t *stack, size_t size);
void wos_task_suspend(void *handle);
void wos_task_resume(void *handle);
void *wos_task_handle(void);

#define WOS_TIMER_PERIODIC 1
#define WOS_TIMER_ONESHOT 0
void *wos_timer_create(uint32_t interval, bool mode, void (*callback)(void *user), void *user);
void wos_timer_free(void *handle);
int wos_timer_start(void *handle);
int wos_timer_started(void *handle);
int wos_timer_stop(void *handle);

int wos_event_get_value(void *handle, bool clear);
int wos_event_get_value_wait(void *handle, uint32_t delay, bool clear);
int wos_event_set_value(void *handle, int event);
int wos_event_set_value_wait(void *handle, uint32_t delay, int event);

void *wos_mutex_create(void);
int wos_mutex_take(void *handle, uint32_t ms);
int wos_mutex_give(void *handle);