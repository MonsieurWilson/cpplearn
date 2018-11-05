#ifndef _LOG_H
#define _LOG_H

#include <spdlog/spdlog.h>


class Log
{
    public:
        bool init(const std::string &logger_name, const std::string &file_name,
                  size_t max_file_size = 1024 * 1024 * 10, size_t max_files = 10);

        void set_log_level(const std::string &log_level);

        static Log &instance(void)
        {
            static Log s_log;
            return s_log;
        }

        std::shared_ptr<spdlog::logger> file_logger;

    private:
        Log();
        ~Log();

        std::unordered_map<std::string, spdlog::level::level_enum> level_map;
};

inline Log &GetLog(void) {
    return Log::instance();
}

template<class ... Args>
inline void LOG_DEBUG(Args... args) {
    GetLog().file_logger->debug(args...);
}

template<class ... Args>
inline void LOG_INFO(Args... args) {
    GetLog().file_logger->info(args...);
}

template<class ... Args>
inline void LOG_WARN(Args... args) {
    GetLog().file_logger->warn(args...);
}

template<class ... Args>
inline void LOG_ERROR(Args... args) {
    GetLog().file_logger->error(args...);
}

template<class ... Args>
inline void LOG_CRITICAL(Args... args) {
    GetLog().file_logger->critical(args...);
}

#endif
