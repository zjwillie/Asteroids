#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "LogEntry.hpp"
#include "LogLevel.hpp"
#include "LogSink.hpp"

// Portable function-name macro
#define LOG_FUNC __func__

namespace LoggerDetail {
    constexpr const char* extractFilename(const char* path) {
        const char* file = path;
        while (*path) {
            if (*path == '/' || *path == '\\') file = path + 1;
            ++path;
        }
        return file;
    }
}

class Logger {
public:
    static void initialize();
    static void shutdown();

    static void addSink(std::unique_ptr<LogSink> sink);
    static void setThreshold(LogLevel level);
    static void setChannelEnabled(const std::string& channel, bool enabled);

    // Called by the macros below. Don't call directly.
    static void log(LogLevel level,
        const std::string& channel,
        const char* file,
        int line,
        const char* function,
        std::string message);

private:
    static double computeTimestamp();

    static std::vector<std::unique_ptr<LogSink>> sinks_;
    static LogLevel threshold_;
    static std::unordered_map<std::string, bool> channelStates_;
    static uint64_t startCounter_;
};

// -----------------------------------------------------------------------------
// Macros: Called from codebase using the logger
// Usage: LOG_INFO("Weapon", "Bullet spawned at {:.1f}, {:.1f}", x, y);
// -----------------------------------------------------------------------------

#define LOG_MASTER(level, channel, ...)                                  \
    Logger::log(                                                         \
        level,                                                           \
        channel,                                                         \
        LoggerDetail::extractFilename(__FILE__),                         \
        __LINE__,                                                        \
        LOG_FUNC,                                                        \
        std::format(__VA_ARGS__)                                         \
    )

#define LOG_DEBUG(channel, ...) LOG_MASTER(LogLevel::Debug, channel, __VA_ARGS__)
#define LOG_INFO(channel, ...)  LOG_MASTER(LogLevel::Info,  channel, __VA_ARGS__)
#define LOG_WARN(channel, ...)  LOG_MASTER(LogLevel::Warn,  channel, __VA_ARGS__)
#define LOG_ERROR(channel, ...) LOG_MASTER(LogLevel::Error, channel, __VA_ARGS__)
#define LOG_FATAL(channel, ...) LOG_MASTER(LogLevel::Fatal, channel, __VA_ARGS__)