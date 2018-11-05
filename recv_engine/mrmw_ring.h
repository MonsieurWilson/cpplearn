//mtmw_ring.h

#ifndef _XDF_MRMW_RING_H_
#define _XDF_MRMW_RING_H_

#include <boost/lockfree/queue.hpp>

#include "lock.h"
#include "condition.h"
#include "configs.h"

namespace xdf
{
    typedef struct _pg_res_mrmw_ring
    {
        xdf::Mutex                                           mutex_mrmw;
        xdf::Condition                                       event_mrmw;
        boost::lockfree::queue<void *, 
              boost::lockfree::fixed_sized<true>, 
              boost::lockfree::capacity<PACKET_GROUP_COUNT>> ring;
    } pg_res_ring;

    typedef struct _pq_res_mrmw_ring
    {
        xdf::Mutex                                           mutex_mrmw;
        xdf::Condition                                       event_mrmw;
        boost::lockfree::queue<void *, 
              boost::lockfree::fixed_sized<true>, 
              boost::lockfree::capacity<PARQUET_MEM_COUNT>>  ring;
    } pq_res_ring;

    template<typename T>
    static inline bool __attribute__((always_inline))
    mrmw_ring_sp_enqueue(T *r, void *const obj)
    {
        return r->ring.push(obj);
    }

    template<typename T>
    static inline void * __attribute__((always_inline))
    mrmw_ring_sc_dequeue(T *r)
    {
        void *obj;
        return r->ring.pop(obj) ? obj : nullptr;
    }
        
    template<typename T>
    static inline void mrmw_ring_wait_event(T *r, int seconds)
    {
        struct timespec ts;
        struct timeval  tv;
        gettimeofday(&tv, 0);
        Scoped_lock slock(r->mutex_mrmw);
        ts.tv_sec = tv.tv_sec + seconds;
        ts.tv_nsec = tv.tv_usec * 1000;
        r->event_mrmw.timed_wait(slock, ts);
    }

    template<typename T>
    static inline void mrmw_ring_notify_one(T *r)
    {
        r->event_mrmw.notify_one();
    }

    template<typename T>
    static inline void mrmw_ring_notify_all(T *r)
    {
        r->event_mrmw.notify_all();
    }

} //namespace xdf

#endif // _XDF_MRMW_RING_H
