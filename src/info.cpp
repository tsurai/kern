#include "info.h"

namespace Kern {
    LogLevel operator|(LogLevel a, LogLevel b) {
        return static_cast<LogLevel>(static_cast<char>(a) | static_cast<char>(b));
    }

    LogLevel operator&(LogLevel a, LogLevel b) {
        return static_cast<LogLevel>(static_cast<char>(a) & static_cast<char>(b));
    }

    LogLevel operator^(LogLevel a, LogLevel b) {
        return static_cast<LogLevel>(static_cast<char>(a) ^ static_cast<char>(b));
    }

    LogLevel operator~(LogLevel a) {
        return static_cast<LogLevel>(~static_cast<char>(a));
    }

    const char *level_to_str(LogLevel &level) {
        switch(level) {
            case LogLevel::Trace:
                return "trace";
            case LogLevel::Debug:
                return "debug";
            case LogLevel::Info:
                return "info";
            case LogLevel::Warning:
                return "warning";
            case LogLevel::Error:
                return "error";
            case LogLevel::Fatal:
                return "fatal";
            default:
                return "unknown";
        }
    }
}
