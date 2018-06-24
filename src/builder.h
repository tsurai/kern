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

    // Builds valid Dispatch objects by modifying the inner Dispatch object
    // one property at a time. The default state is guaranteed to be valid.
    // Completing an object build exhausts the DispatchBuilder and renders it
    // unable to build more objects.
    // The building methods filter, format, level, sink and chain return the
    // DispatchBuilder object as a reference to allow method chaining.
    class DispatchBuilder {
    public:
        DispatchBuilder();
        ~DispatchBuilder();

        DispatchBuilder(DispatchBuilder&&);
        DispatchBuilder& operator=(DispatchBuilder&&);

        // Prevent copy operations.
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
        // Holds the Dispatch object to be build.
        Dispatch *inner;
    };
}

#endif //BUILDER_H
