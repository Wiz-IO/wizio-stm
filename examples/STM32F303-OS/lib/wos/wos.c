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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "wos.h"
#include "swTimer.h"

#ifdef CM4F
#define OS_REG_CNT (16 + 17)
#else
#define OS_REG_CNT (16)
#endif

typedef enum
{
    OS_TASK_STATUS_IDLE = 0,
    OS_TASK_STATUS_ACTIVE,
    OS_TASK_STATUS_BLOCKED,
    OS_TASK_STATUS_SUSPEND
} os_task_status_e;

typedef enum
{
    OS_STATE_DEFAULT = 0,
    OS_STATE_INITIALIZED,
    OS_STATE_STARTED,
    OS_STATE_STOPPED
} os_state_e;

typedef struct node_s
{
    uint32_t data;
    struct node_s *next;
} node_t;

typedef struct os_task_s
{
    volatile uint32_t sp; // must be first
    void (*entry)(void *params);
    void *params;
    os_task_status_e status;
    int events;
    void *next;
} os_task_t;

volatile os_task_t *os_curr_task = NULL;
volatile os_task_t *os_next_task = NULL;

static struct
{
    os_task_t *list;
    int count; // tasks in list
#ifdef OS_CONFIG_TIMERS
    os_task_t *timer;
#endif
    os_state_e state;
    uint32_t ticks;
} os = {0};

void os_load();

static void task_end(void)
{
    int i = 0;
    while (1)
        i++;
}

static void *os_task_construct(void (*entry)(void *params), void *params, uint32_t *stack, size_t size, bool add)
{
    os_task_t *task = NULL;
    if (entry && stack && size)
    {
        task = (os_task_t *)calloc(sizeof(os_task_t), 1);
        if (task)
        {
            uint32_t offset = (size / sizeof(uint32_t));
            // task->status = OS_TASK_STATUS_IDLE; // = 0
            task->entry = entry;
            task->params = params;
            task->sp = (uint32_t)(stack + offset - OS_REG_CNT);
            stack[offset - 1] = 0x01000000;             // XPSR
            stack[offset - 2] = (uint32_t)entry & ~1UL; // PC
            stack[offset - 3] = (uint32_t)&task_end;    // LR
            stack[offset - 8] = (uint32_t)params;       // R0
            if (add)
            {
                if (os.list)
                {
                    volatile os_task_t *p = os.list;
                    __disable_irq();
                    while (p->next)
                        p = p->next;
                    p->next = task;
                    __enable_irq();
                }
                else
                {
                    os.list = task;
                }
                os.count++;
            }
        }
    }
    return task;
}

int os_init(void *params)
{
#ifdef OS_CONFIG_TIMERS
    void os_timers_entry(void *params);
    static uint32_t stack[OS_CONFIG_MINIMAL_STACK_SIZE];
    os.timer = os_task_construct(os_timers_entry, params, stack, sizeof(stack), false);
    assert(os.timer);
#endif
    os_load();
    os.state = OS_STATE_INITIALIZED;
    return 0;
}

void *os_task_create(void (*entry)(void *params), void *params, uint32_t *stack, size_t size)
{
    os_task_t *task = NULL;
    if ((OS_STATE_INITIALIZED == os.state) && entry && size && (size % sizeof(uint32_t) == 0) && size > OS_REG_CNT)
    {
        task = (stack) ? os_task_construct(entry, params, stack, size, true) : os_task_construct(entry, params, (uint32_t *)malloc(size), size, true);
    }
    return task;
}

void os_start(void)
{
    assert(os.list);
    assert(OS_STATE_INITIALIZED == os.state);
    {
        __disable_irq();

        NVIC_SetPriority(PendSV_IRQn, 0xff);  // Lowest possible priority
        NVIC_SetPriority(SVCall_IRQn, 0x00);  // Highest possible priority
        NVIC_SetPriority(SysTick_IRQn, 0x00); // Highest possible priority

        os_curr_task = os.list; // First task

        volatile os_task_t *p = os.list;
        while (p->next)
            p = p->next;
        p->next = os.list; // connect the list

        os.state = OS_STATE_STARTED;
        __enable_irq();

        __set_PSP(os_curr_task->sp + (OS_REG_CNT * sizeof(uint32_t))); // Set PSP to the top of task stack
        __set_CONTROL(0x03);                                           // Switch to Unprivilleged Thread Mode with PSP
        __ISB();

        os_curr_task->entry(os_curr_task->params);
    }
    assert(0);
}

void os_delay(uint32_t ms)
{
    if (os.state > OS_STATE_INITIALIZED)
    {
        if (ms)
        {
            os_curr_task->status = OS_TASK_STATUS_BLOCKED;
            uint32_t t = HAL_GetTick();
            while (HAL_GetTick() - t < ms)
            {
                HAL_Delay(1);
                os_yield();
            }
            os_curr_task->status = OS_TASK_STATUS_ACTIVE;
        }
        else
        {
            os_yield();
        }
    }
    else
    {
        HAL_Delay(ms);
    }
}

void os_suspend(void) { os.state = OS_STATE_STOPPED; }

void os_resume(void) { os.state = OS_STATE_STARTED; }

void os_task_suspend(void *handle)
{
#ifdef OS_CONFIG_TIMERS
    if (os_curr_task == os.timer)
        return;
#endif
    if (handle)
        ((os_task_t *)handle)->status = OS_TASK_STATUS_SUSPEND;
    else
        os_curr_task->status = OS_TASK_STATUS_SUSPEND;
}

void os_task_resume(void *handle)
{
    if (handle)
        ((os_task_t *)handle)->status = OS_TASK_STATUS_IDLE;
    else
        os_curr_task->status = OS_TASK_STATUS_ACTIVE;
}

void *os_task_handle(void) { return (void *)os_curr_task; }

///////////////////////////////////////////////////////////////////////////////

volatile void *os_switch(void)
{
    os_curr_task->status = OS_TASK_STATUS_IDLE;
    os_next_task->status = OS_TASK_STATUS_ACTIVE;
    return (os_curr_task = os_next_task);
}

static os_task_t *os_next(void)
{
    os_task_t *p = os_curr_task->next;
    for (int i = 0; i < os.count; i++)
    {
        if (p->status == OS_TASK_STATUS_IDLE)
            break;
        p = p->next;
    }
    return p;
}

static void SVC(int number)
{
    if (OS_STATE_STARTED == os.state)
    {
        switch (number)
        {
        case 0: // yield
            os_next_task = os_next();
            break;

#ifdef OS_CONFIG_TIMERS
        case 1:
            if (os_curr_task == os.timer)
                return;
            os.timer->next = (os_task_t *)os_curr_task;
            os_next_task = os.timer;
            break;

        case 2:
            os_next_task = os.timer->next; // return to task
            break;
#endif

        default:
            return;
        }

        if (os_next_task)
            SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; // Trigger PendSV which performs the actual context switch
    }
}

void SVC_Handler_Main(unsigned int *svc_args)
{
    SVC(((char *)svc_args[6])[-2]);
}

void SVC_Handler(void)
{
    __asm(".global SVC_Handler_Main	\n"
          "TST lr, #4				\n"
          "ITE EQ					\n"
          "MRSEQ r0, MSP			\n"
          "MRSNE r0, PSP			\n"
          "B SVC_Handler_Main		\n");
}

#ifdef OS_CONFIG_PREEMPTION
static bool preemption = 0;
#endif

void SysTick_Handler(void)
{
    HAL_IncTick();

#ifdef OS_CONFIG_SWITCH_TIME
#if OS_CONFIG_SWITCH_TIME > 1
    if (++os.ticks % OS_CONFIG_SWITCH_TIME == 0)
#endif
#endif
    {
        if (OS_STATE_STARTED == os.state)
        {
#ifdef OS_CONFIG_PREEMPTION
#ifdef OS_CONFIG_TIMERS
            SVC(preemption);
            preemption = !preemption;
#else
            SVC(0); /* only tasks */
#endif
#else
#ifdef OS_CONFIG_TIMERS
            SVC(1); /* timer */
#endif
#endif
        }
    }
}

// TIMERS /////////////////////////////////////////////////////////////////////
#ifdef OS_CONFIG_TIMERS

void os_timers_entry(void *params)
{
    while (1)
    {
        os.state = OS_STATE_STOPPED;
        swTimerTaskHandler();
        if (params)
            ((void (*)(void))params)();
        os.state = OS_STATE_STARTED;
        asm volatile("svc #2"); // return to the tasks
    }
}

static void task_timers_handler(void *params)
{
    while (1)
    {
        swTimerTaskHandler();   // check timers
        asm volatile("svc #2"); // return to current task
    }
}

static void timer_cb(struct swTimer_t *timer)
{
    if (timer)
        if (timer->callback)
            timer->callback(timer->user);
}

void os_timer_free(void *handle)
{
    if (handle)
    {
        os_timer_stop(handle);
        free(handle);
    }
}

void *os_timer_create(uint32_t interval, bool mode, void (*callback)(void *user), void *user)
{
    swTimer_t *p = NULL;
    if (interval && callback)
    {
        if ((p = calloc(sizeof(swTimer_t), 1)))
        {
            p->interval = interval;
            p->mode = (swTimerMode_t)mode;
            p->user = user;
            p->callback = callback;
            p->cb = timer_cb;
        }
    }
    return p;
}

int os_timer_start(void *handle)
{
    if (handle)
    {
        swTimerStart((swTimer_t *)handle);
        return 0;
    }
    return -1;
}

int os_timer_stop(void *handle)
{
    if (handle)
    {
        swTimerStop((swTimer_t *)handle);
        return 0;
    }
    return -1;
}

int os_timer_started(void *handle)
{
    return (handle) ? swTimerStarted((swTimer_t *)handle) : -1;
}

#endif

// EVENTS /////////////////////////////////////////////////////////////////////
#ifdef OS_CONFIG_EVENTS

int os_event_get(void *handle, bool clear)
{
    int res = -1;
    if (handle)
    {
        __disable_irq();
        res = ((os_task_t *)handle)->events;
        if (clear)
            ((os_task_t *)handle)->events = 0;
        __enable_irq();
    }
    return res;
}

int os_event_set(void *handle, int event)
{
    int res = -1;
    if (handle)
    {
        __disable_irq();
        ((os_task_t *)handle)->events = event;
        __enable_irq();
        res = 0;
    }
    return res;
}

int os_event_get_wait(void *handle, uint32_t delay, bool clear)
{
    int res = -1; // error
    uint32_t t = HAL_GetTick();
    do
    {
        if ((res = os_event_get(handle, clear)))
            return res;
        HAL_Delay(1);
        os_yield();
    } while (HAL_GetTick() - t < delay);
    res = -2; // timeout
    return res;
}

int os_event_set_wait(void *handle, uint32_t delay, int event)
{
    int res = -1; // error
    uint32_t t = HAL_GetTick();
    do
    {
        if (0 == os_event_get(handle, false))
            return os_event_set(handle, event);
        HAL_Delay(1);
        os_yield();
    } while (HAL_GetTick() - t < delay);
    res = -2; // timeout
    return res;
}

#endif

// MUTEX //////////////////////////////////////////////////////////////////////
typedef struct
{
    void *owner;
    int val;
} sem_t;

#ifdef OS_CONFIG_MUTEX

void *os_mutex_create(void)
{
    return calloc(sizeof(uint32_t), 1);
}

int os_mutex_take(void *handle, uint32_t ms)
{
    if (handle && os.state > OS_STATE_INITIALIZED)
    {
        uint32_t t = HAL_GetTick();
        do
        {
            if (0 == ((sem_t *)handle)->val || ((sem_t *)handle)->owner == os_curr_task)
            {
                __disable_irq();
                ((sem_t *)handle)->val = (int)(((sem_t *)handle)->owner = (void *)os_curr_task);
                __enable_irq();
                return 0;
            }
            HAL_Delay(1);
            os_yield();
        } while (HAL_GetTick() - t < ms);
    }
    return -1;
}

int os_mutex_give(void *handle)
{
    if (handle && os.state > OS_STATE_INITIALIZED)
    {
        if (((sem_t *)handle)->val)
        {
            if (((sem_t *)handle)->owner == os_curr_task)
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