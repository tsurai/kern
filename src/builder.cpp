#include "builder.h"

namespace kern {
    DispatchBuilder::DispatchBuilder() {
        // The constructor of Dispatch guarantees a valid default state.
        this->inner = new Dispatch();
    }

    DispatchBuilder::~DispatchBuilder() {
        if(this->inner != nullptr)
            delete inner;
    }

    DispatchBuilder &DispatchBuilder::filter(FnFilter func) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        this->inner->filter_func = func;

        return *this;
    }

    DispatchBuilder &DispatchBuilder::format(FnFormat func) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        if(func == nullptr)
            throw std::invalid_argument("function ptr argument is NULL");

        this->inner->format_func = func;
        this->inner->is_def_format = false;

        return *this;
    }

    // The argument is being used as a bitflag and should contain all level
    // that the Dispatch should log.
    DispatchBuilder &DispatchBuilder::level(LogLevel level) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        this->inner->log_level = level;
        this->inner->is_def_level = false;

        return *this;
    }

    // The argument is being used to calculate a bitmask filtering all level
    // below the given level.
    DispatchBuilder &DispatchBuilder::min_level(LogLevel level) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        this->inner->log_level = LogLevel::All ^ static_cast<LogLevel>(static_cast<char>(level) - 1);
        this->inner->is_def_level = false;

        return *this;
    }


    DispatchBuilder &DispatchBuilder::sink(std::unique_ptr<Sink> sink) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        if(sink == nullptr)
            throw std::invalid_argument("sink argument is NULL");

        this->inner->output_sink = std::move(sink);

        return *this;
    }

    // Adds a child Dispatch to the current Dispatch objects chain vector.
    // The child inherits every non default properties of its parent unless
    // already otherwise changed.
    DispatchBuilder &DispatchBuilder::chain(std::unique_ptr<Dispatch> dispatch) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        if(dispatch == nullptr)
            throw std::invalid_argument("dispatch argument is NULL");

        // inherit format function
        if(!this->inner->is_def_format && dispatch->is_def_format)
            dispatch->format_func = this->inner->format_func;

        // inherit log level
        if(!this->inner->is_def_level && dispatch->is_def_level)
            dispatch->log_level = this->inner->log_level;

        this->inner->chain.push_back(std::move(dispatch));

        return *this;
    }

    // Returns the completely built Dispatch object. Calling this method will
    // exhaust the Builder preventing any more objects from being built.
    std::unique_ptr<Dispatch> DispatchBuilder::build() {
         if(this->inner == nullptr)
            throw BuilderReuseException();

        auto d = std::unique_ptr<Dispatch>(this->inner);

        // Marks the Builder as being exhausted.
        this->inner = nullptr;

        return d;
    }

    // Assign the built object to the global Dispatch causing all logging
    // macros to use this Dispatch. The call to the build() method exhausts
    // the DispatchBuilder object.
    void DispatchBuilder::apply() {
        auto d = this->build();
        Dispatch::global_dispatch.swap(d);
    }
}
