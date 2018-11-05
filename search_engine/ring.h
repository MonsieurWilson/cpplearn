#ifndef _RING_H
#define _RING_H

#include <mutex>
#include <condition_variable>
#include <boost/lockfree/queue.hpp>

#include "log.h"


template <unsigned int RINGSIZE>
class Ring 
{
    public:
        Ring() {}

        ~Ring() {}

        /*
         * Name:
         *  Ring::push
         *
         * Description:
         *  This function is used to enqueue element into boost lockfree queue.
         *
         * Parameters:
         *  obj        - [IN] element to be put (PayloadBlock instance).
         *  wait_timed - [IN] whether invoke condition and time wait.
         *  time_out   - [IN] seconds for time out. (only useful when wait_timed is true).
         *  
         * Returns:
         *  true, success.
         *  false, failure.
         *
         * NOTE:
         *  If the given parameter `wait_timed` is true, then the function will
         *  block `time_out` seconds and retry until the quene is available to enqueue.
         *  On the other hand, if `wait_timed` is false, the function will
         *  ignore the `time_out` input.
         */
        bool push(void *obj, bool wait_timed, unsigned int time_out);

        /* 
         * Name:
         *  Ring::pop
         * 
         * Description:
         *  This function is used to dequeue element from boost lockfree queue.
         *
         * Parameters:
         *  wait_timed - [IN] whether invoke condition and time wait.
         *  time_out   - [IN] seconds for time out. (only useful when wait_timed is true).
         * 
         * Returns:
         *  nullptr, failed to pop.
         *  others, success.
         * 
         * NOTE:
         *  If the given parameter `wait_timed` is true, then the function will
         *  block `time_out` seconds and retry until the quene is available to dequeue.
         *  On the other hand, if `wait_timed` is false, the function will
         *  ignore the `time_out` input.
         */
        void *pop(bool wait_timed, unsigned int time_out);

        Ring(const Ring &r) = delete;

        Ring &operator=(const Ring &r) = delete;

    private:
        boost::lockfree::queue<void *, 
                               boost::lockfree::fixed_sized<true>,
                               boost::lockfree::capacity<RINGSIZE>> r;
        std::mutex mtx;
        std::condition_variable cv;
};

template <unsigned int RINGSIZE>
bool Ring<RINGSIZE>::push(void *obj, bool wait_timed, unsigned int time_out) {
    if (!wait_timed) {
        return r.push(obj);
    }

    while (!r.push(obj)) {
        std::unique_lock<std::mutex> lck(mtx);
        if (cv.wait_for(lck, std::chrono::seconds(time_out)) 
               == std::cv_status::timeout) {
            LOG_DEBUG("wait for Ring::push");
        }
    }
    cv.notify_all();
    return true;
}

template <unsigned int RINGSIZE>
void *Ring<RINGSIZE>::pop(bool wait_timed, unsigned int time_out) {
    void *obj = nullptr;
    if (!wait_timed) {
        return r.pop(obj) ? obj : nullptr;
    }

    while (!r.pop(obj)) {
        std::unique_lock<std::mutex> lck(mtx);
        if (cv.wait_for(lck, std::chrono::seconds(time_out)) 
               == std::cv_status::timeout) {
            LOG_DEBUG("wait for Ring::pop");
        }
    }
    cv.notify_all();
    return obj;
}

#endif
