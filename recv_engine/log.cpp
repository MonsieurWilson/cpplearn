#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "log.h"


Log::Log() {}

Log::~Log() {
    spdlog::drop_all();
}

bool Log::init(const std::string &logger_name, const std::string &file_name,
               size_t max_file_size, size_t max_files) {

    try {
        file_logger = spdlog::rotating_logger_mt(logger_name + "file_logger",
                                                 file_name, 
                                                 max_file_size,
                                                 max_files);
#ifdef DEBUG
        spdlog::set_level(spdlog::level::debug);
        file_logger->flush_on(spdlog::level::debug);
#else
        spdlog::set_level(spdlog::level::info);
        file_logger->flush_on(spdlog::level::info);
#endif
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S (%t)] [%l] %v");
    } catch (const spdlog::spdlog_ex &ex) {
        std::cout << "Log init failed: " << ex.what() << std::endl;
        return false;
    }

    return true;
}

void Log::set_log_level(const std::string &log_level) {
    std::unordered_map<std::string, spdlog::level::level_enum> level_map {
        {"debug",    spdlog::level::debug}, 
        {"info",     spdlog::level::info}, 
        {"warn",     spdlog::level::warn}, 
        {"error",    spdlog::level::err}, 
        {"critical", spdlog::level::critical}, 
        {"off",      spdlog::level::off}};
    std::string level = log_level;
    std::transform(level.begin(), level.end(), level.begin(), tolower);
    auto pos = level_map.find(level);
    if (pos != level_map.end()) {
        spdlog::set_level(pos->second);
        file_logger->flush_on(pos->second);
    } else {
        file_logger->error("Invalid log level!");
    }
}

