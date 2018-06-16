#ifndef LOGGER_H
#define LOGGER_H

#include "builder.h"

namespace Kern {
    #define log(lvl, msg) Dispatch::get_logger()->write(lvl, __FILE__, __func__, __LINE__, msg)
    #define trace(x) log(LogLevel::Trace, x)
    #define debug(x) log(LogLevel::Debug, x)
    #define info(x) log(LogLevel::Info, x)
    #define warning(x) log(LogLevel::Warning, x)
    #define error(x) log(LogLevel::Error, x)
    #define fatal(x) log(LogLevel::Fatal, x)
}

#endif //LOGGER_H
