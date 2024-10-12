#pragma once

#include <memory>

#include "tape-interface.hpp"

namespace NTape {
    void Sort(std::unique_ptr<ITape> input, std::unique_ptr<ITape> output);
}
