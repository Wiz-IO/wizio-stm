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

int os_init(void *);
void os_start(void);
void os_suspend(void);
void os_resume(void);
void os_delay(uint32_t ms);
static inline void os_yield(void) { asm volatile("svc #0"); }

void *os_task_create(void (*entry)(void *params), void *params, uint32_t *stack, size_t size);
void os_task_suspend(void *handle);
void os_task_resume(void *handle);
void *os_task_handle(void);

#define OS_TIMER_PERIODIC 1
#define OS_TIMER_ONESHOT 0
void *os_timer_create(uint32_t interval, bool mode, void (*callback)(void *user), void *user);
void os_timer_free(void *handle);
int os_timer_start(void *handle);
int os_timer_started(void *handle);
int os_timer_stop(void *handle);

int os_event_get(void *handle, bool clear);
int os_event_get_wait(void *handle, uint32_t delay, bool clear);
int os_event_set(void *handle, int event);
int os_event_set_wait(void *handle, uint32_t delay, int event);

void *os_mutex_create(void);
int os_mutex_take(void *handle, uint32_t ms);
int os_mutex_give(void *handle);