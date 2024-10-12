#include <string>
#include "tape-interface.hpp"

namespace NTape {

class TTapeFile final : ITape {
private:
    uint32_t pos;
    size_t size;
    FILE* file;

    static const size_t normalizeCoefficient = 19;
    static std::string normilize(uint32_t);

    ~TTapeFile();
public:
    TTapeFile();
    TTapeFile(std::string);

    void Delete() override;
    bool MoveLeft() override;
    bool MoveRight() override;
    uint32_t ReadUInt32() const override;
    void WriteUInt32(uint32_t) override;
};

} // namespace Tape
