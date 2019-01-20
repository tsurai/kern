#ifndef LOGGER_H
#define LOGGER_H

#include "builder.h"
#include "dispatch.h"

namespace kern {
    #define _log(lvl, ...) Dispatch::get_logger()->write(lvl, __FILE__, __func__, __LINE__, __VA_ARGS__)

    #define trace(...) kern::_log(kern::LogLevel::Trace, __VA_ARGS__)
    #define debug(...) kern::_log(kern::LogLevel::Debug, __VA_ARGS__)
    #define info(...) kern::_log(kern::LogLevel::Info, __VA_ARGS__)
    #define warning(...) kern::_log(kern::LogLevel::Warning, __VA_ARGS__)
    #define error(...) kern::_log(kern::LogLevel::Error, __VA_ARGS__)
    #define fatal(...) kern::_log(kern::LogLevel::Fatal, __VA_ARGS__)
}

#endif //LOGGER_H
