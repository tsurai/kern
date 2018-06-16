#include "builder.h"

namespace Kern {
    DispatchBuilder::DispatchBuilder() {
        this->inner = new Dispatch();
    }

    DispatchBuilder::~DispatchBuilder() {
        if(this->inner != nullptr)
            delete inner;
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

    DispatchBuilder &DispatchBuilder::filter(LogLevel level) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        this->inner->log_level = level;
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

    DispatchBuilder &DispatchBuilder::chain(std::unique_ptr<Dispatch> dispatch) {
        if(this->inner == nullptr)
            throw BuilderReuseException();

        if(dispatch == nullptr)
            throw std::invalid_argument("dispatch argument is NULL");

        // inherit format if not otherwise specified
        if(!this->inner->is_def_format && dispatch->is_def_format)
            dispatch->format_func = this->inner->format_func;

        // inherit format if not otherwise specified
        if(!this->inner->is_def_level && dispatch->is_def_level)
            dispatch->log_level = this->inner->log_level;

        this->inner->dchain.push_back(std::move(dispatch));

        return *this;
    }

    std::unique_ptr<Dispatch> DispatchBuilder::build() {
         if(this->inner == nullptr)
            throw BuilderReuseException();

        auto d = std::unique_ptr<Dispatch>(this->inner);
        this->inner = nullptr;

        return d;
    }

    void DispatchBuilder::apply() {
        auto d = this->build();
        Dispatch::global_dispatch.swap(d);
    }
}
