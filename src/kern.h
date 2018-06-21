#ifndef LOGGER_H
#define LOGGER_H

#include "builder.h"
#include "dispatch.h"

namespace Kern {
    #define log(lvl, fmt, ...) Dispatch::get_logger()->write(lvl, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
    #define trace(fmt, ...) log(LogLevel::Trace, fmt, ##__VA_ARGS__)
    #define debug(fmt, ...) log(LogLevel::Debug, fmt, ##__VA_ARGS__)
    #define info(fmt, ...) log(LogLevel::Info, fmt, ##__VA_ARGS__)
    #define warning(fmt, ...) log(LogLevel::Warning, fmt, ##__VA_ARGS__)
    #define error(fmt, ...) log(LogLevel::Error, fmt, ##__VA_ARGS__)
    #define fatal(fmt, ...) log(LogLevel::Fatal, fmt, ##__VA_ARGS__)
}

#endif //LOGGER_H
