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
 *      TEST/DEDUG Version ( there may be bugs )
 *
 ******************************************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h> // TODO: find short solution
#include "wos.h"
#include "swTimer.h"

#ifdef CM4F
#define WOS_REGISTERS (16 + 17)
#else
#define WOS_REGISTERS (16)
#endif

typedef enum
{
    WOS_TASK_STATUS_IDLE = 0,
    WOS_TASK_STATUS_ACTIVE,
    WOS_TASK_STATUS_BLOCKED,
    WOS_TASK_STATUS_SUSPEND
} wos_task_status_e;

typedef enum
{
    WOS_STATE_DEFAULT = 0,
    WOS_STATE_INITIALIZED,
    WOS_STATE_STARTED,
    WOS_STATE_STOPPED
} wos_state_e;

typedef struct node_s
{
    uint32_t data;
    struct node_s *next;
} node_t;

typedef struct wos_task_s
{
    volatile uint32_t sp;        // must be first
    void (*entry)(void *params); // task entry
    void *params;                // task params
    int event;
    wos_task_status_e status;
    void *next;
} wos_task_t;

volatile wos_task_t *wos_curr_task = NULL;
volatile wos_task_t *wos_next_task = NULL;

static struct
{
    wos_task_t *list;
    int count; // list
#ifdef WOS_CONFIG_TIMERS
    wos_task_t *timer;
#endif
    wos_state_e state;
    uint32_t ticks;
} wos = {0};

static void wos_task_end(void)
{
    int i = 0;
    while (1)
        i++;
}

static void *wos_task_construct(void (*entry)(void *params), void *params, uint32_t *stack, size_t size, bool add)
{
    wos_task_t *task = NULL;
    if (entry && stack && size)
    {
        task = (wos_task_t *)calloc(sizeof(wos_task_t), 1);
        if (task)
        {
            uint32_t offset = (size / sizeof(uint32_t));
            // task->status = WOS_TASK_STATUS_IDLE; // = 0
            task->entry = entry;
            task->params = params;
            task->sp = (uint32_t)(stack + offset - WOS_REGISTERS);
            stack[offset - 1] = 0x01000000;              // XPSR
            stack[offset - 2] = (uint32_t)entry & ~1UL;  // PC
            stack[offset - 3] = (uint32_t)&wos_task_end; // LR
            stack[offset - 8] = (uint32_t)params;        // R0
            if (add)
            {
                __disable_irq();
                if (wos.list)
                {
                    volatile wos_task_t *p = wos.list;
                    while (p->next)
                        p = p->next;
                    p->next = task; // append task
                }
                else
                {
                    wos.list = task;
                }
                wos.count++;
                __enable_irq();
            }
        }
    }
    return task;
}

static void wos_timers_entry(void *params);
int wos_init(void *params)
{
    wos_dumy();
#ifdef WOS_CONFIG_TIMERS
    static uint32_t stack[WOS_CONFIG_MINIMAL_STACK_SIZE];
    wos.timer = wos_task_construct(wos_timers_entry, params, stack, sizeof(stack), false);
    assert(wos.timer);
#endif
    wos.state = WOS_STATE_INITIALIZED;
    return 0;
}

void *wos_task_create(void (*entry)(void *params), void *params, uint32_t *stack, size_t size)
{
    wos_task_t *task = NULL;
    if ((WOS_STATE_INITIALIZED == wos.state) && entry && size && (size % sizeof(uint32_t) == 0) && size > WOS_REGISTERS)
    {
        task = (stack) ? wos_task_construct(entry, params, stack, size, true) : wos_task_construct(entry, params, (uint32_t *)malloc(size), size, true);
    }
    return task;
}

void wos_start(void)
{
    assert(wos.list);
    assert(WOS_STATE_INITIALIZED == wos.state);
    {
        __disable_irq();

        NVIC_SetPriority(PendSV_IRQn, 0xff);  // Lowest possible priority
        NVIC_SetPriority(SVCall_IRQn, 0x00);  // Highest possible priority
        NVIC_SetPriority(SysTick_IRQn, 0x00); // Highest possible priority

        wos_curr_task = wos.list; // First task

        volatile wos_task_t *p = wos.list;
        while (p->next)
            p = p->next;
        p->next = wos.list; // connect the list

        wos.state = WOS_STATE_STARTED;

        __enable_irq();

        __set_PSP(wos_curr_task->sp + (WOS_REGISTERS * sizeof(uint32_t))); // Set PSP to the top of task stack
        __set_CONTROL(0x03);                                               // Switch to Unprivilleged Thread Mode with PSP
        __ISB();

        wos_curr_task->entry(wos_curr_task->params);
    }
    assert(0);
}

void wos_delay(uint32_t ms)
{
    if (wos.state > WOS_STATE_INITIALIZED)
    {
        if (ms)
        {
            wos_curr_task->status = WOS_TASK_STATUS_BLOCKED;
            uint32_t begin = HAL_GetTick();
            while (HAL_GetTick() - begin < ms)
            {
                wos_yield();
            }
            wos_curr_task->status = WOS_TASK_STATUS_ACTIVE;
        }
        else
        {
            wos_yield();
        }
    }
    else
    {
        HAL_Delay(ms);
    }
}

void wos_suspend(void) { wos.state = WOS_STATE_STOPPED; }

void wos_resume(void) { wos.state = WOS_STATE_STARTED; }

void wos_task_suspend(void *handle)
{
#ifdef WOS_CONFIG_TIMERS
    if (wos_curr_task == wos.timer)
        return;
#endif
    if (handle)
        ((wos_task_t *)handle)->status = WOS_TASK_STATUS_SUSPEND;
    else
        wos_curr_task->status = WOS_TASK_STATUS_SUSPEND;
}

void wos_task_resume(void *handle)
{
    if (handle)
        ((wos_task_t *)handle)->status = WOS_TASK_STATUS_IDLE;
    else
        wos_curr_task->status = WOS_TASK_STATUS_ACTIVE;
}

void *wos_task_handle(void) { return (void *)wos_curr_task; }

///////////////////////////////////////////////////////////////////////////////

volatile void *wos_switch(void)
{
    wos_curr_task->status = WOS_TASK_STATUS_IDLE;
    wos_next_task->status = WOS_TASK_STATUS_ACTIVE;
    return (wos_curr_task = wos_next_task);
}

static wos_task_t *wos_next(void)
{
    wos_task_t *p = wos_curr_task->next;
    for (int i = 0; i < wos.count; i++)
    {
        if (p->status == WOS_TASK_STATUS_IDLE)
            break;
        p = p->next;
    }
    return p;
}

static void SVC(int number)
{
    if (WOS_STATE_STARTED == wos.state)
    {
        switch (number)
        {
        case 0: // yield
            wos_next_task = wos_next();
            break;

#ifdef WOS_CONFIG_TIMERS
        case 1: // from task to timers
            if (wos_curr_task == wos.timer)
                return;
            wos.timer->next = (wos_task_t *)wos_curr_task;
            wos_next_task = wos.timer;
            break;

        case 2: // from timers to task
            wos_next_task = wos.timer->next;
            break;
#endif

        default:
            return;
        }

        if (wos_next_task)
            SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Trigger PendSV which performs the actual context switch
    }
}

void SVC_Handler_Main(uint32_t *svc_args)
{
    SVC(((char *)svc_args[6])[-2]);
}

void SVC_Handler(void)
{
    __asm(".global SVC_Handler_Main \n"
          "TST lr, #4               \n"
          "ITE EQ                   \n"
          "MRSEQ r0, MSP            \n"
          "MRSNE r0, PSP            \n"
          "B SVC_Handler_Main       \n");
}

#ifdef WOS_CONFIG_PREEMPTION
static bool preemption = 0;
#endif

void SysTick_Handler(void)
{
    HAL_IncTick();

#ifdef WOS_CONFIG_SWITCH_TIME
#if WOS_CONFIG_SWITCH_TIME > 1
    if (++wos.ticks % WOS_CONFIG_SWITCH_TIME == 0)
#endif
#endif
    {
        if (WOS_STATE_STARTED == wos.state)
        {
#ifdef WOS_CONFIG_PREEMPTION
#ifdef WOS_CONFIG_TIMERS
            SVC(preemption);
            preemption = !preemption;
#else
            SVC(0);
#endif
#else // NO WOS_CONFIG_PREEMPTION
#ifdef WOS_CONFIG_TIMERS
            SVC(1);
#endif
#endif
        }
    }
}

// TIMERS /////////////////////////////////////////////////////////////////////

#ifdef WOS_CONFIG_TIMERS

static void wos_timers_entry(void *params)
{
    while (1)
    {
        wos.state = WOS_STATE_STOPPED;
        swTimerTaskHandler(); // check all timers
        if (params)
            ((void (*)(void))params)();
        wos.state = WOS_STATE_STARTED;
        asm volatile("svc #2"); // return to the tasks
    }
}

static void wos_timer_cb(struct swTimer_t *timer)
{
    if (timer)
        if (timer->callback)
            timer->callback(timer->user);
}

void wos_timer_free(void *handle)
{
    if (handle)
    {
        wos_timer_stop(handle);
        free(handle);
    }
}

void *wos_timer_create(uint32_t interval, bool mode, void (*user_callback)(void *), void *user_params)
{
    swTimer_t *p = NULL;
    if (interval && user_callback)
    {
        if ((p = calloc(sizeof(swTimer_t), 1)))
        {
            p->interval = interval;
            p->mode = (swTimerMode_t)mode;
            p->user = user_params;
            p->callback = user_callback;
            p->cb = wos_timer_cb;
        }
    }
    return p;
}

int wos_timer_start(void *handle)
{
    if (handle)
    {
        swTimerStart((swTimer_t *)handle);
        return 0;
    }
    return -1;
}

int wos_timer_stop(void *handle)
{
    if (handle)
    {
        swTimerStop((swTimer_t *)handle);
        return 0;
    }
    return -1;
}

int wos_timer_started(void *handle)
{
    return (handle) ? swTimerStarted((swTimer_t *)handle) : -1;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//      From here on down is untested and are subject to change              //
///////////////////////////////////////////////////////////////////////////////

// EVENTS /////////////////////////////////////////////////////////////////////

#ifdef WOS_CONFIG_EVENTS

int wos_event_get_value(void *handle, bool clear)
{
    int res = -1;
    if (handle && wos.state > WOS_STATE_INITIALIZED)
    {
        __disable_irq();
        res = ((wos_task_t *)handle)->event;
        if (clear)
            ((wos_task_t *)handle)->event = 0;
        __enable_irq();
    }
    return res;
}

int wos_event_set_value(void *handle, int event)
{
    int res = -1;
    if (handle && wos.state > WOS_STATE_INITIALIZED)
    {
        __disable_irq();
        ((wos_task_t *)handle)->event = event;
        __enable_irq();
        res = 0;
    }
    return res;
}

int wos_event_get_value_wait(void *handle, uint32_t delay, bool clear) // if not zero
{
    int res = -1;
    uint32_t t = HAL_GetTick();
    do
    {
        if ((res = wos_event_get_value(handle, clear)))
            return res;
        wos_yield();
    } while (HAL_GetTick() - t < delay);
    return res;
}

int wos_event_set_value_wait(void *handle, uint32_t delay, int event) // if is zero
{
    int res = -1;
    uint32_t begin = HAL_GetTick();
    do
    {
        if (0 == wos_event_get_value(handle, false))
            return wos_event_set_value(handle, event);
        wos_yield();
    } while (HAL_GetTick() - begin < delay);
    return res;
}

// TODO BITS

#endif

// MUTEX //////////////////////////////////////////////////////////////////////

typedef struct
{
    void *owner;
    uint32_t value;
    uint32_t count;
} sem_t;

#ifdef WOS_CONFIG_MUTEX

void *wos_mutex_create(void)
{
    return calloc(sizeof(sem_t), 1);
}

int wos_mutex_take(void *handle, uint32_t ms)
{
    if (handle && wos.state > WOS_STATE_INITIALIZED)
    {
        sem_t *h = (sem_t *)handle;
        if (0 == h->count) // mutex ?
        {
            uint32_t begin = HAL_GetTick();
            do
            {
                if (0 == h->value)
                {
                    __disable_irq();
                    h->value = (uint32_t)(h->owner = (void *)wos_curr_task);
                    __enable_irq();
                    return 0; // ok
                }
                HAL_Delay(1);
                wos_yield();
            } while (HAL_GetTick() - begin < ms);
        }
    }
    return -1;
}

int wos_mutex_give(void *handle)
{
    if (handle && wos.state > WOS_STATE_INITIALIZED)
    {
        sem_t *h = (sem_t *)handle;
        if (0 == h->count) // mutex
        {
            if (h->value && (h->owner == wos_curr_task) && (h->owner == h->value))
            {
                __disable_irq();
                memset(handle, 0, sizeof(sem_t));
                __enable_irq();
                return 0;
            }
        }
    }
    return -1;
}

#endif

// SEMAPHORE //////////////////////////////////////////////////////////////////

#ifdef WOS_CONFIG_SEMAPHORE

void *wos_sem_create(uint32_t count)
{
    sem_t *h = (sem_t *)calloc(sizeof(sem_t), 1);
    if (h)
        h->value = h->count = count ? count : 1;
    return h;
}

int wos_sem_take(void *handle, uint32_t ms)
{
    if (handle && wos.state > WOS_STATE_INITIALIZED)
    {
        sem_t *h = (sem_t *)handle;
        if (h->count) // mutex ?
        {
            // if (h->owner == wos_curr_task) return 0; // recursive ???

            if (0 == h->value) // block
            {
                uint32_t begin = HAL_GetTick();
                do
                {
                    if (h->value)
                        goto done;
                    wos_yield();
                } while (HAL_GetTick() - begin < ms);
            }
            else
            {
            done:
                __disable_irq();
                h->owner == wos_curr_task;
                h->value--;
                __enable_irq();
                return 0;
            }
        }
    }
    return -1;
}

int wos_sem_give(void *handle)
{
    if (handle && wos.state > WOS_STATE_INITIALIZED)
    {
        sem_t *h = (sem_t *)handle;
        if (h->count) // mutex ?
        {
            // if (h->owner == wos_curr_task) return 0; // recursive ???

            if (h->value < h->count)
            {
                __disable_irq();
                h->value++;
                h->owner == wos_curr_task;
                __enable_irq();
                return 0;
            }
        }
    }
    return -1;
}

#endif
