#include <stdint.h>
#include "base-interfaces.hpp"

namespace NTape {

class ITape : NBaseInterfaces::IBase {
protected:
    virtual ~ITape() = default;
public:
    virtual void MoveLeft() = 0;
    virtual void MoveRight() = 0;
    virtual uint32_t ReadUInt32() const = 0;
    virtual void WriteUInt32(uint32_t) = 0;
};

} // namespace NTape
