//rw_ring.h

#ifndef _XDF_RW_RING_H_
#define _XDF_RW_RING_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <atomic>

#include "lock.h"
#include "condition.h"
#include "configs.h"

namespace xdf
{

    typedef struct _rw_ring
    {
        uint32_t                size;           // Size of the ring.
        uint32_t                mask;           //< Mask (size-1) of ring.
        std::atomic<uint32_t>   cons_indx;
        std::atomic<uint32_t>   prod_indx;
        xdf::Mutex              mutex_rw;
        xdf::Condition          event_rw;
        void*                   objs[0] __attribute__((__aligned__(64)));    // Memory space of ring starts here.
    } rw_ring;

    /* true if x is a power of 2 */
    #define POWEROF2(x) ((((x)-1) & (x)) == 0)
    #define RTE_RING_SZ_MASK  (unsigned)(0x0fffffff) /**< Ring size mask */

    static inline rw_ring* create_rw_ring(size_t count)
    {
        rw_ring* ring = nullptr;
        /* count must be a power of 2 */
        if( (!POWEROF2(count)) || (count > RTE_RING_SZ_MASK) ) {
            printf("Requested size is invalid, must be power of 2, and do not exceed the size limit %u\n", RTE_RING_SZ_MASK);
            return nullptr;
        }
        size_t ring_size = sizeof(rw_ring) + count * sizeof(void *);
        void* data = malloc(ring_size);
        if( nullptr != data ) {
            ring = (rw_ring*)data;
            memset(ring, 0, sizeof(*ring));
            ring->size = count;
            ring->mask = count -1;
            ring->prod_indx = 0;
            ring->cons_indx = 0;
        } else {
            printf("Cannot reserve memory");
            return nullptr;
        }

        return ring;
    }

    static inline uint32_t __attribute__((always_inline))
    rw_ring_entries(rw_ring* r)
    {
        return r->prod_indx - r->cons_indx;
    }

    static inline uint32_t __attribute__((always_inline))
    rw_ring_free_entries(rw_ring* r)
    {
        return r->size + r->cons_indx - r->prod_indx;
    }

    static inline void destory_rw_ring(rw_ring* &r)
    {
        if( nullptr != r ) {
            free((void*)r);
            r = nullptr;
        }
    }

    static inline bool __attribute__((always_inline))
    rw_ring_sp_enqueue(rw_ring* r, void* const obj)
    {
        if( 0 == rw_ring_free_entries(r) ) {
            return false;
        } else {
            r->objs[r->prod_indx++ & r->mask] = obj;
            return true;
        }
    }

    static inline void* __attribute__((always_inline))
    rw_ring_sc_dequeue(rw_ring* r)
    {
        void* obj = nullptr;
        if( 0 != rw_ring_entries(r) ) {
            obj = r->objs[r->cons_indx++ & r->mask];
        }
        return obj;
    }

    static inline void rw_ring_wait_event(rw_ring* r, int seconds)
    {
        struct timespec ts;
        struct timeval  tv;
        gettimeofday(&tv, 0);
        Scoped_lock slock(r->mutex_rw);
        ts.tv_sec = tv.tv_sec + seconds;
        ts.tv_nsec = tv.tv_usec * 1000;
        r->event_rw.timed_wait(slock, ts);
    }

    static inline void rw_ring_notify_one(rw_ring* r)
    {
        r->event_rw.notify_one();
    }

} //namespace xdf

#endif  //_XDF_RW_RING_H_
