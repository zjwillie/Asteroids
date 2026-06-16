#include "FileSink.hpp"

#include <format>

FileSink::FileSink(const std::string& filepath) {
    stream_.open(filepath, std::ios::out | std::ios::trunc);
}

FileSink::~FileSink() {
    if (stream_.is_open()) {
        stream_.close();
    }
}

void FileSink::write(const LogEntry& entry) {
    if (!stream_.is_open()) return;

    std::string line = std::format(
        "[{:.3f}] [{}] [{}] ({}:{} in {}) {}\n",
        entry.timestamp,
        levelToString(entry.level),
        entry.channel,
        entry.file,
        entry.line,
        entry.function,
        entry.message
    );

    stream_ << line;
    stream_.flush();
}