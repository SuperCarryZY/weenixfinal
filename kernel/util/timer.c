#include "util/timer.h"
#include "proc/spinlock.h"
#include "util/time.h"

static timer_t *timer_running = NULL;
static uint64_t timer_next_expiry = -1;
static list_t timers_primary = LIST_INITIALIZER(timers_primary);
static list_t timers_secondary = LIST_INITIALIZER(timers_secondary);
static int timers_firing = 0;

void timer_init(timer_t *timer)
{
    timer->expires = -1;
    list_link_init(&timer->link);
}

void timer_add(timer_t *timer) { timer_mod(timer, timer->expires); }

int __timer_del(timer_t *timer)
{
    int ret = 0;
    if (list_link_is_linked(&timer->link))
    {
        list_remove(&timer->link);
        ret = 1;
    }
    return ret;
}

int timer_del(timer_t *timer)
{
    int ret = __timer_del(timer);

    return ret;
}

void __timer_add(timer_t *timer)
{
    KASSERT(!list_link_is_linked(&timer->link));
    list_t *list = timers_firing ? &timers_secondary : &timers_primary;
    list_insert_head(list, &timer->link);
}

int timer_mod(timer_t *timer, int expires)
{

    timer->expires = expires;
    int ret = __timer_del(timer);
    __timer_add(timer);
    timer_next_expiry = MIN(timer_next_expiry, timer->expires);

    return ret;
}

int timer_pending(timer_t *timer)
{
    int ret = list_link_is_linked(&timer->link);
    return ret;
}

int timer_del_sync(timer_t *timer)
{
    /* Not great performance wise... */
    while (timer_running == timer)
    {
        sched_yield();
    }

    int ret = __timer_del(timer);

    return ret;
}

/* Note: using a linked-list rather than some priority is terribly inefficient
 * Also this implementation is just bad. Sorry.
 */
int ready = 0;
void __timers_fire()
{
    if (curthr && !preemption_enabled())
    {
        return;
    }

    timers_firing = 1;

    //dbg(DBG_PRINT, "next expiry: %d\n", timer_next_expiry);
    if (jiffies < timer_next_expiry)
    {
        timers_firing = 0;
        return;
    }

    uint64_t min_expiry = 0;

    list_iterate(&timers_primary, timer, timer_t, link)
    {
        if (jiffies >= timer->expires)
        {
            list_remove(&timer->link);
            timer_running = timer;
            timer->function(timer->data);
            timer_running = NULL;
        }
        else
        {
            min_expiry = MIN(min_expiry, timer->expires);
        }
    }

    /* migrate from the backup list to the primary list */
    list_iterate(&timers_secondary, timer, timer_t, link)
    {
        min_expiry = MIN(min_expiry, timer->expires);
        list_remove(&timer->link);
        list_insert_head(&timers_primary, &timer->link);
    }

    timer_next_expiry = min_expiry;
    timers_firing = 0;
}
