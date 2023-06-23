#ifndef _swTIMER_H_
#define _swTIMER_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

  typedef enum
  {
    ONESHOT,
    PERIODIC,
  } swTimerMode_t;

  typedef struct swTimer_t
  {
    // private
    struct swTimer_t *next;
    uint32_t timeout;

    // public
    uint32_t interval;
    swTimerMode_t mode;
    void (*cb)(struct swTimer_t *timer);
    void * user;
    void (*callback)(void *user);
  } swTimer_t;

  void swTimerTaskHandler(void) __attribute__((weak));
  void swTimerStart(swTimer_t *timer);
  void swTimerStop(swTimer_t *timer);
  bool swTimerStarted(swTimer_t *timer);

  swTimer_t *swTimer(uint32_t interval, swTimerMode_t mode, void (*cb)(struct swTimer_t *timer));

#ifdef __cplusplus
}
#endif
#endif // _swTIMER_H_