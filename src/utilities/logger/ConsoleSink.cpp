#include <cstdio>
#include <format>

#include "ConsoleSink.hpp"

void ConsoleSink::write(const LogEntry& entry) {
    std::string line = std::format(
        "[{:.3f}] [{}] [{}] ({}:{}) {}\n",
        entry.timestamp,
        levelToString(entry.level),
        entry.channel,
        entry.file,
        entry.line,
        entry.message
    );

    std::fputs(line.c_str(), stdout);
    std::fflush(stdout);
}