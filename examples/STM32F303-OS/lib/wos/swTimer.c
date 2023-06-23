#include <stdint.h>
#include "swTimer.h"

static swTimer_t *sTimers = NULL;

static void placeTimer(swTimer_t *timer)
{
    // asm volatile("cpsid i");
    if (sTimers)
    {
        swTimer_t *prev = NULL;
        uint32_t timeout = timer->interval;

        for (swTimer_t *t = sTimers; t; t = t->next)
        {
            if (timeout < t->timeout)
            {
                t->timeout -= timeout;
                break;
            }
            else
                timeout -= t->timeout;

            prev = t;
        }

        timer->timeout = timeout;

        if (prev)
        {
            timer->next = prev->next;
            prev->next = timer;
        }
        else
        {
            timer->next = sTimers;
            sTimers = timer;
        }
    }
    else
    {
        timer->next = NULL;
        timer->timeout = timer->interval;
        sTimers = timer;
    }
    // asm volatile("cpsie i");
}

void swTimerStart(swTimer_t *timer)
{
    if (!swTimerStarted(timer))
        placeTimer(timer);
}

void swTimerStop(swTimer_t *timer)
{
    swTimer_t *prev = NULL;
    for (swTimer_t *t = sTimers; t; t = t->next)
    {
        if (t == timer)
        {
            if (prev)
                prev->next = t->next;
            else
                sTimers = t->next;

            if (t->next)
                t->next->timeout += timer->timeout;

            break;
        }
        prev = t;
    }
}

bool swTimerStarted(swTimer_t *timer)
{
    for (swTimer_t *t = sTimers; t; t = t->next)
        if (t == timer)
            return true;
    return false;
}

void swTimerTaskHandler(void)
{
    extern uint32_t HAL_GetTick(void);
    if (sTimers)
    {
        static uint32_t oldTime = 0;
        uint32_t elapsed = HAL_GetTick() - oldTime;
        if (elapsed)
        {
            oldTime = HAL_GetTick();
            while (sTimers && (sTimers->timeout <= elapsed))
            {
                swTimer_t *timer = sTimers;
                elapsed -= sTimers->timeout;
                sTimers = sTimers->next;
                if (PERIODIC == timer->mode)
                    placeTimer(timer);
                if (timer->cb)
                    timer->cb(timer);
            }
            if (sTimers)
                sTimers->timeout -= elapsed;
        }
    }
}
