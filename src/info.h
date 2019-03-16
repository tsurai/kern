#ifndef INFO_H
#define INFO_H

namespace kern {
    // Various log levels to allow targeted redirection and formatting of
    // log messages
    enum class LogLevel : char {
        Trace   = 1 << 0,
        Debug   = 1 << 1,
        Info    = 1 << 2,
        Warning = 1 << 3,
        Error   = 1 << 4,
        Fatal   = 1 << 5,
        All     = (1 << 6) - 1
    };

    // Operator for Bitflag like behavior
    LogLevel operator|(LogLevel, LogLevel) noexcept;
    LogLevel operator&(LogLevel, LogLevel) noexcept;
    LogLevel operator^(LogLevel, LogLevel) noexcept;
    LogLevel operator~(LogLevel) noexcept;
    LogLevel& operator|= (LogLevel &, LogLevel) noexcept;
    LogLevel& operator&= (LogLevel &, LogLevel) noexcept;

    const char *level_to_str(const LogLevel &) noexcept;

    struct Metadata {
        LogLevel level;
        const char *level_str;
        const char *file;
        const char *function;
        int line;
    };
}

#endif //INFO_H
