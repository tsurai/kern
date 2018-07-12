#ifndef LOGGER_H
#define LOGGER_H

#include "builder.h"
#include "dispatch.h"

namespace kern {
    #define _log(lvl, ...) Dispatch::get_logger()->write(lvl, __FILE__, __func__, __LINE__, __VA_ARGS__)

    #define trace(...) _log(LogLevel::Trace, __VA_ARGS__)
    #define debug(...) _log(LogLevel::Debug, __VA_ARGS__)
    #define info(...) _log(LogLevel::Info, __VA_ARGS__)
    #define warning(...) _log(LogLevel::Warning, __VA_ARGS__)
    #define error(...) _log(LogLevel::Error, __VA_ARGS__)
    #define fatal(...) _log(LogLevel::Fatal, __VA_ARGS__)
}

#endif //LOGGER_H
