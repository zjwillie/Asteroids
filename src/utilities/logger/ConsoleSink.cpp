#include <cstdio>
#include <format>
#include <string_view>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "ConsoleSink.hpp"

namespace {

    constexpr std::string_view colorReset = "\033[0m";

    std::string_view colorForLevel(LogLevel level) {
        switch (level) {
        case LogLevel::Debug: return "\033[90m";    // gray
        case LogLevel::Info:  return "\033[34m";    // blue
        case LogLevel::Warn:  return "\033[33m";    // yellow
        case LogLevel::Error: return "\033[31m";    // red
        case LogLevel::Fatal: return "\033[1;95m";  // bold pink (bright magenta)
        }
        return "";
    }

    void enableAnsiOnWindows() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;
        DWORD mode = 0;
        if (!GetConsoleMode(hOut, &mode)) return;
        SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    }

} // anonymous namespace

ConsoleSink::ConsoleSink() {
    enableAnsiOnWindows();
    printColorTest();
}

void ConsoleSink::write(const LogEntry& entry) {
    std::string line = std::format(
        "[{:.3f}] [{}{}{}] [{}] ({}:{} in {}) {}\n",
        entry.timestamp,
        colorForLevel(entry.level),
        levelToString(entry.level),
        colorReset,
        entry.channel,
        entry.file,
        entry.line,
        entry.function,
        entry.message
    );

    std::fputs(line.c_str(), stdout);
    std::fflush(stdout);
}

void ConsoleSink::printColorTest() {
    std::fputs("\n", stdout);
    std::fputs("=== Logger color test ===\n", stdout);

    auto demo = [](LogLevel level, const char* desc) {
        std::string line = std::format(
            "  [{}{}{}] {}\n",
            colorForLevel(level),
            levelToString(level),
            colorReset,
            desc
        );
        std::fputs(line.c_str(), stdout);
        };

    demo(LogLevel::Debug, "gray  - fine details, per-frame chatter");
    demo(LogLevel::Info, "white - lifecycle events, normal operation");
    demo(LogLevel::Warn, "yellow - recovered problems, suspicious state");
    demo(LogLevel::Error, "red   - failed operations");
    demo(LogLevel::Fatal, "bold red - unrecoverable, game stopping");

    std::fputs("=========================\n\n", stdout);
    std::fflush(stdout);
}