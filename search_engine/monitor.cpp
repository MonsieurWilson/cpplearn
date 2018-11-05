#include <chrono>
#include <unistd.h>

#include "monitor.h"
#include "config.h"
#include "task_info.h"
#include "utils.h"
#include "config.h"


Monitor::Monitor() : m_timer(m_io), m_interval(1) {}

Monitor::~Monitor() {}

void Monitor::monitor_loop() {
    m_timer.expires_from_now(std::chrono::minutes(m_interval));
    m_timer.async_wait(std::bind(&Monitor::cleaness, this));

    m_io.run();
}

void Monitor::cleaness() {
    auto bptr = GetGlobalVars()->task_pool.begin();
    time_t old, now = time(NULL);

    for (; bptr != GetGlobalVars()->task_pool.end();) {
        /* skip running task no matter how long it has been running */
        if (0 == strcmp("RUNNING", bptr->second.get_str_val_by_key("status"))) {
            LOG_DEBUG("- [monitor] Task {} is still running, skip.",
                      bptr->second.get_str_val_by_key("uuid"));
            ++bptr;
            continue;
        }

        const char *ctime_ptr = bptr->second.get_str_val_by_key("ctime");
        LOG_DEBUG("- [monitor] Task {} ctime = <{}>.",
                  bptr->second.get_str_val_by_key("uuid"),
                  ctime_ptr);
        old = strftime_to_timestamp(ctime_ptr);
        if (now - old >= GetGlobalVars()->task_stale_interval * 60) {
            LOG_INFO("- [monitor] Task {} become stale and is going to be removed.",
                     bptr->second.get_str_val_by_key("uuid"));
            unlink(bptr->second.get_str_val_by_key("file_name"));
            GetGlobalVars()->task_pool.erase(bptr++);
        } else {
            ++bptr;
        }
    }

    /* next loop */
    m_timer.expires_from_now(std::chrono::minutes(m_interval));
    m_timer.async_wait(std::bind(&Monitor::cleaness, this));
}

