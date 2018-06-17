#ifndef BUILDER_H
#define BUILDER_H

#include <exception>
#include "dispatch.h"

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

        DispatchBuilder &format(FnFormat func);
        DispatchBuilder &level(LogLevel);
        DispatchBuilder &sink(std::unique_ptr<Sink> sink);
        DispatchBuilder &chain(std::unique_ptr<Dispatch> dispatch);
        std::unique_ptr<Dispatch> build();
        void apply();
    private:
        Dispatch *inner;
    };
}

#endif //BUILDER_H
