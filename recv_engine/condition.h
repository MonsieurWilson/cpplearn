//condition.h

#ifndef _XDF_CONDITION_H_
#define _XDF_CONDITION_H_

#include <stdexcept>
#include <cassert>
#include <errno.h>

#include "noncopyable.h"

#ifdef WIN32
#include <windows.h>

namespace xdf 
{
    class Condition : private Noncopyable
    {
    public:
        Condition() { }
        ~Condition() { }

        void notify_one() { }
        void notify_all() { }
    };
} //namespace xdf

#else

#include <pthread.h>

namespace xdf
{
    class Condition_impl : private Noncopyable
    {
    public:
        Condition_impl()
        {
            int ret = 0;
            ret = pthread_cond_init(&m_Condition, 0);
            ret = ret;
            assert(0 == ret);
        }
        ~Condition_impl()
        {
            int ret = 0;
            ret = pthread_cond_destroy(&m_Condition);
            ret = ret;
            //assert(0 == ret);
        }

        void notify_one()
        {
            int ret = 0;
            ret = pthread_cond_signal(&m_Condition);
            ret = ret;
            assert(0 == ret);
        }
        void notify_all()
        {
            int ret = 0;
            ret = pthread_cond_broadcast(&m_Condition);
            ret = ret;
            assert(0 == ret);
        }

        void do_wait(pthread_mutex_t *pmutex)
        {
            int ret = 0;
            ret = pthread_cond_wait(&m_Condition, pmutex);
            ret = ret;
            assert(0 == ret);
        }

        bool do_timed_wait(const timespec &ts, pthread_mutex_t *pmutex)
        {
            int ret = 0;
            ret = pthread_cond_timedwait(&m_Condition, pmutex, &ts);
            assert(0 == ret || ETIMEDOUT == ret);
            return (ETIMEDOUT != ret);
        }

    public:
        pthread_cond_t m_Condition;
    };

    class Condition : private Noncopyable
    {
    public:
        Condition() { }
        ~Condition() { }

        void notify_one() { m_impl.notify_one(); }
        void notify_all() { m_impl.notify_all(); }

        template <typename L>
        void wait(L &lock)
        {
            if( !lock )
                throw std::logic_error("lock error");;        
            do_wait(lock.m_mutex);
        }

        template <typename L>
        bool timed_wait(L &lock, const timespec &ts)
        {
            if( !lock )
                throw std::logic_error("lock error");;        
            return do_timed_wait(lock.m_mutex, ts);
        }

    private:
        Condition_impl m_impl;

        template <typename M>
        void do_wait(M &mutex)
        {
            pthread_mutex_t *pmutex = mutex.get_detail();
            m_impl.do_wait(pmutex);
        }

        template <typename M>
        bool do_timed_wait(M &mutex, const timespec &ts)
        {
            pthread_mutex_t *pmutex = mutex.get_detail();
            bool ret = false;
            ret = m_impl.do_timed_wait(ts, pmutex);
            return ret;
        }
    }; 

} //namespace xdf

#endif //WIN32

#endif //_XDF_CONDITION_H_
