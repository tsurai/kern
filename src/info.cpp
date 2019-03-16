#include "info.h"

namespace kern {
    LogLevel operator|(LogLevel lhs, LogLevel rhs) noexcept {
        return static_cast<LogLevel>(static_cast<char>(lhs) | static_cast<char>(rhs));
    }

    LogLevel operator&(LogLevel lhs, LogLevel rhs) noexcept {
        return static_cast<LogLevel>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }

    LogLevel operator^(LogLevel lhs, LogLevel rhs) noexcept {
        return static_cast<LogLevel>(static_cast<char>(lhs) ^ static_cast<char>(rhs));
    }

    LogLevel operator~(LogLevel rhs) noexcept {
        return static_cast<LogLevel>(~static_cast<char>(rhs));
    }

    LogLevel& operator|= (LogLevel &lhs, LogLevel rhs) noexcept {
        return lhs = lhs | rhs;
    }

    LogLevel& operator&= (LogLevel &lhs, LogLevel rhs) noexcept{
        return lhs = lhs & rhs;
    }

    const char *level_to_str(const LogLevel &level) noexcept {
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
