#include "info.h"

namespace Kern {
    LogLevel operator|(LogLevel lhs, LogLevel rhs) {
        return static_cast<LogLevel>(static_cast<char>(lhs) | static_cast<char>(rhs));
    }

    LogLevel operator&(LogLevel lhs, LogLevel rhs) {
        return static_cast<LogLevel>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }

    LogLevel operator^(LogLevel lhs, LogLevel rhs) {
        return static_cast<LogLevel>(static_cast<char>(lhs) ^ static_cast<char>(rhs));
    }

    LogLevel operator~(LogLevel rhs) {
        return static_cast<LogLevel>(~static_cast<char>(rhs));
    }

    const char *level_to_str(const LogLevel &level) {
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
                return "N/A";
        }
    }
}
