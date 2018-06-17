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

    LogLevel operator|(LogLevel a, LogLevel b);
    LogLevel operator&(LogLevel a, LogLevel b);
    LogLevel operator^(LogLevel a, LogLevel b);
    LogLevel operator~(LogLevel a);

    const char *level_to_str(LogLevel &a);
}

#endif //INFO_H
