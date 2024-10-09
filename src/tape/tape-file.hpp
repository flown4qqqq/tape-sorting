#include "interfaces.hpp"

namespace Tape {

class TTapeFile final : ITape {
private:
    ~TTapeFile();
    TTapeFile();
public:
    void Delete() override;
    void MoveLeft() override;
    void MoveRight() override;
    uint32_t ReadUInt32() const override;
    void WriteUInt32(uint32_t) override;
};

} // namespace Tape
