#ifndef BUILDER_H
#define BUILDER_H

#include <exception>

#include "dispatch.h"
#include "info.h"
#include "sink.h"

namespace Kern {
    class BuilderReuseException : public std::exception {
        const char *what() const throw() {
            return "Reuse of consumed builder";
        }
    };

    class DispatchBuilder {
    public:
        DispatchBuilder();
        ~DispatchBuilder();

        DispatchBuilder(const DispatchBuilder &) = delete;
        DispatchBuilder& operator=(const DispatchBuilder &) = delete;

        DispatchBuilder &filter(FnFilter);
        DispatchBuilder &format(FnFormat);
        DispatchBuilder &level(LogLevel);
        DispatchBuilder &sink(std::unique_ptr<Sink>);
        DispatchBuilder &chain(std::unique_ptr<Dispatch>);
        std::unique_ptr<Dispatch> build();
        void apply();
    private:
        Dispatch *inner;
    };
}

#endif //BUILDER_H
