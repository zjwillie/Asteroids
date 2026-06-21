#include "pch.h"

#include "Logger.hpp"

#include <SDL3/SDL.h>

#include "ConsoleSink.hpp"

// Static member definitions
std::vector<std::unique_ptr<LogSink>> Logger::sinks_;
LogLevel Logger::threshold_ = LogLevel::Debug;
std::unordered_map<std::string, bool> Logger::channelStates_;
uint64_t Logger::startCounter_ = 0;

void Logger::initialize() {
    startCounter_ = SDL_GetPerformanceCounter();
    sinks_.push_back(std::make_unique<ConsoleSink>());
}

void Logger::shutdown() {
    sinks_.clear();
}

void Logger::addSink(std::unique_ptr<LogSink> sink) {
    sinks_.push_back(std::move(sink));
}

void Logger::setThreshold(LogLevel level) {
    threshold_ = level;
}

void Logger::setChannelEnabled(const std::string& channel, bool enabled) {
    channelStates_[channel] = enabled;
}

double Logger::computeTimestamp() {
    uint64_t now = SDL_GetPerformanceCounter();
    uint64_t freq = SDL_GetPerformanceFrequency();
    return static_cast<double>(now - startCounter_) / static_cast<double>(freq);
}

void Logger::log(LogLevel level,
    const std::string& channel,
    const char* file,
    int line,
    const char* function,
    std::string message) {
    // Threshold filter
    if (level < threshold_) return;

    // Channel filter - default-enabled; only mute when explicitly disabled
    auto it = channelStates_.find(channel);
    if (it != channelStates_.end() && !it->second) return;

    LogEntry entry{
        .timestamp = computeTimestamp(),
        .level = level,
        .channel = channel,
        .file = file,
        .line = line,
        .function = function,
        .message = std::move(message)
    };

    for (auto& sink : sinks_) {
        sink->write(entry);
    }
}