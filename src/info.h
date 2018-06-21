#ifndef INFO_H
#define INFO_H

namespace Kern {
    enum class LogLevel : char {
        Trace   = 1 << 0,
        Debug   = 1 << 1,
        Info    = 1 << 2,
        Warning = 1 << 3,
        Error   = 1 << 4,
        Fatal   = 1 << 5,
        All     = (1 << 6) - 1
    };

    struct Metadata {
        LogLevel level;
        const char *level_str;
        const char *file;
        const char *function;
        int line;
    };

    LogLevel operator|(LogLevel, LogLevel);
    LogLevel operator&(LogLevel, LogLevel);
    LogLevel operator^(LogLevel, LogLevel);
    LogLevel operator~(LogLevel);

    const char *level_to_str(const LogLevel &);
}

#endif //INFO_H
