#ifndef _MONITOR_H
#define _MONITOR_H

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>


class Monitor 
{
    public:
        Monitor();

        ~Monitor();

        /* 
         * Name:
         *  Monitor::monitor_loop
         * 
         * Description:
         *  This is the monitor entry function.
         *
         * Parameters:
         *  None.
         * 
         * Returns:
         *  None.
         * 
         * NOTE:
         *  None.
         */
        void monitor_loop();

    private:
        void cleaness();

        boost::asio::io_service   m_io;
        boost::asio::steady_timer m_timer;
        unsigned int              m_interval;
};

#endif
