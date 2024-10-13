#include "sort.hpp"

#include <iostream>

namespace NTape {
    void Sort(TUniquePtr<ITape>& input, TUniquePtr<ITape>& output, size_t maxMemory) {
        std::cout << &input << std::endl;
        std::cout << &output << std::endl;
        std::cout << maxMemory << std::endl;
    }
}
