#pragma once

#include <memory>

#include "tape-interface.hpp"

namespace NTape {
    template <class I>
    using TUniquePtr = std::unique_ptr<I, NBaseInterfaces::IBase::BaseDeleter>;

    void Sort(TUniquePtr<ITape>& input, TUniquePtr<ITape>& output, size_t maxMemory);
}
