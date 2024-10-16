#pragma once

#include <string>
#include <random>

#include "tape-interface.hpp"
#include "sort.hpp"

namespace NTape {

class TTapeFile final : ITape {
private:
    static std::mt19937 randomGenerator;

    size_t pos;
    size_t size;
    FILE* file;
    std::string filePath;

    static const int32_t normalizeCoefficient = 19;
    static std::string normalize(uint32_t);

    ~TTapeFile();
public:
    TTapeFile();
    TTapeFile(std::string);

    void Delete() override;

    bool MoveLeft() override;
    bool MoveRight(bool forced = false) override;
    bool SetCurrentPosition(size_t) override;
    size_t GetCurrentPosition() const override;

    uint32_t ReadUInt32() const override;
    bool WriteUInt32(uint32_t, bool forced = false) override;

    size_t Size() const override;

    static ITape* Create();
    static ITape* Create(std::string path);
};

void Sort(std::string input, std::string output, size_t maxMemory);

} // namespace Tape
