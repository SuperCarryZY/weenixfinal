#include "globals.h"
#include "main/apic.h"

void spinlock_init(spinlock_t *lock) { lock->s_locked = 0; }

inline void spinlock_lock(spinlock_t *lock)
{
// __sync_bool_compare_and_swap is a GCC intrinsic for atomic compare-and-swap
// If lock->locked is 0, then it is set to 1 and __sync_bool_compare_and_swap
// returns true Otherwise, lock->locked is left at 1 and
// __sync_bool_compare_and_swap returns false
}

inline void spinlock_unlock(spinlock_t *lock)
{
}

inline long spinlock_ownslock(spinlock_t *lock)
{
    return 1;
}
