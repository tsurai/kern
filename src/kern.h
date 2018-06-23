#ifndef LOGGER_H
#define LOGGER_H

#include "builder.h"
#include "dispatch.h"

namespace Kern {
    #define log(lvl, ...) Dispatch::get_logger()->write(lvl, __FILE__, __func__, __LINE__, __VA_ARGS__)

    #define trace(...) log(LogLevel::Trace, __VA_ARGS__)
    #define debug(...) log(LogLevel::Debug, __VA_ARGS__)
    #define info(...) log(LogLevel::Info, __VA_ARGS__)
    #define warning(...) log(LogLevel::Warning, __VA_ARGS__)
    #define error(...) log(LogLevel::Error, __VA_ARGS__)
    #define fatal(...) log(LogLevel::Fatal, __VA_ARGS__)
}

#endif //LOGGER_H
