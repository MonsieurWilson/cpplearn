//lock.h

#ifndef _XDF_LOCK_H_
#define _XDF_LOCK_H_

#include <stdexcept>
#include <string>
#include <pthread.h>

#include "noncopyable.h"

namespace xdf
{
    class Mutex : private Noncopyable
    {
    public:
        Mutex()  { pthread_mutex_init(&m_mutex, 0); }
        ~Mutex() { pthread_mutex_destroy(&m_mutex); }
        void do_lock() { pthread_mutex_lock(&m_mutex); }
        void do_unlock() { pthread_mutex_unlock(&m_mutex); }
        pthread_mutex_t* get_detail(void) { return &m_mutex; }

    private:
        pthread_mutex_t m_mutex;
    };


    struct lock_ops
    {
        static void lock(Mutex &m)
        {
            m.do_lock();
        }
        static void unlock(Mutex &m)
        {
            m.do_unlock();
        }
    };

    class Scoped_lock : private Noncopyable
    {
    public:
        explicit Scoped_lock(Mutex &mx, bool initially_locked = true)
            : m_mutex(mx), m_locked(false)
        {
            if( initially_locked )
                lock();
        }
        ~Scoped_lock()
        {
            if( m_locked )
                unlock();
        }

        void lock()
        {
            if( m_locked )
                throw std::logic_error("exception: internal logic");
            lock_ops::lock(m_mutex);
            m_locked = true;
        }
        void unlock()
        {
            if( !m_locked )
                throw std::logic_error("exception: internal logic");
            lock_ops::unlock(m_mutex);
            m_locked = false;
        }

        bool locked() const { return m_locked; }
        operator const void*() const { return m_locked ? this : 0; }

    private:
        friend class Condition;
 
        Mutex &m_mutex;
        bool  m_locked;
    }; 

} //namespace xdf

#endif //_XDF_LOCK_H_
