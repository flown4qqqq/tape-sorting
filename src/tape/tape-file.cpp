#include <ctime>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "tape-file.hpp"

namespace NTape {

std::mt19937 TTapeFile::randomGenerator(static_cast<unsigned long>(std::time(nullptr)));

std::string TTapeFile::normalize(uint32_t n) {
    auto result = std::to_string(n);
    while (result.size() < normalizeCoefficient) {
        result = "0" + result;
    }

    return result;
}

TTapeFile::~TTapeFile() {
    fclose(file);
    std::filesystem::remove(this->filePath);
}

TTapeFile::TTapeFile()
    : pos(0)
    , size(0)
{
    std::string path = "/tmp/tape-sort-";

    for (size_t i = 0; i < 20; i++) {
        char c = 'a' + randomGenerator() % ('z' - 'a' + 1);
        path += c;
    }

    this->file = fopen(path.c_str(), "w+");
    this->filePath = path;
}

TTapeFile::TTapeFile(std::string path)
    : TTapeFile()
{
    auto isDigit = [](char c) {
        return '0' <= c && c <= '9';
    };

    FILE* from = fopen(path.c_str(), "r+");

    bool empty = true;
    uint32_t num = 0;
    for (auto c = static_cast<char>(fgetc(from)); ; c = static_cast<char>(fgetc(from))) {
        if (isDigit(c)) {
            num *= 10;
            num += static_cast<uint32_t>(c - '0');
            empty = false;
        } else {
            if (!empty) {
                empty = true;
                auto normNum = normalize(num);
                num = 0;
                this->size++;
                fputs(normNum.c_str(), this->file);
                fputs("\n", this->file);
            }

            if (c == EOF) {
                break;
            }
        }
    }

    fclose(from);
    fseek(this->file, 0, SEEK_SET);
}

void TTapeFile::Delete() {
    delete this;
}

ITape* TTapeFile::Create() {
    return static_cast<ITape*>(new TTapeFile());
}

ITape* TTapeFile::Create(std::string path) {
    return static_cast<ITape*>(new TTapeFile(path));
}


bool TTapeFile::MoveLeft() {
    if (this->pos == 0) {
        return false;
    }

    this->pos--;
    fseek(file, -(normalizeCoefficient + 1), SEEK_CUR);
    return true;
}

bool TTapeFile::MoveRight(bool forced) {
    if (!forced && this->pos == this->size) {
        return false;
    }

    if (this->pos == this->size) {
        this->pos++;
        this->size++;
        auto normZero = normalize(uint32_t()) + "\n";
        fputs(normZero.c_str(), this->file);
    } else {
        this->pos++;
        fseek(file, normalizeCoefficient + 1, SEEK_CUR);
    }

    return true;
}

bool TTapeFile::SetCurrentPosition(size_t pos) {
    if (pos > this->size) {
        return false;
    }

    this->pos = pos;
    fseek(file, static_cast<int32_t>((normalizeCoefficient + 1) * pos), SEEK_SET);
    return true;
}

size_t TTapeFile::GetCurrentPosition() const {
    return this->pos;
}

uint32_t TTapeFile::ReadUInt32() const {
    if (this->pos == this->size) {
        throw new std::out_of_range("TTapeFile::ReadUInt32: pos equals size");
    }

    std::string num;
    num.reserve(normalizeCoefficient);

    for (size_t i = 0; i < normalizeCoefficient; i++) {
        num.push_back(static_cast<char>(fgetc(this->file)));
    }

    fseek(file, -normalizeCoefficient, SEEK_CUR);

    return static_cast<uint32_t>(std::stoull(num)); // there is no stoui or smth, i don't know why
}

bool TTapeFile::WriteUInt32(uint32_t num, bool forced) {
    if (!forced && this->pos == this->size) {
        return false;
    }

    std::string normNum = normalize(num);

    if (this->pos == this->size) {
        this->size++;
        normNum += "\n";
        fputs(normNum.c_str(), this->file);
        fseek(file, -(normalizeCoefficient + 1), SEEK_CUR);
    } else {
        fputs(normNum.c_str(), this->file);
        fseek(file, -normalizeCoefficient, SEEK_CUR);
    }

    return true;
}

size_t TTapeFile::Size() const {
    return this->size;
}



void Sort(std::string input, std::string output, size_t maxMemory) {
    TUniquePtr<ITape> sorted(TTapeFile::Create(input));
    TUniquePtr<ITape> temp1(TTapeFile::Create());
    TUniquePtr<ITape> temp2(TTapeFile::Create());
    sorted->ReadUInt32();
    Sort(sorted, temp1, temp2, maxMemory);

    std::ofstream out;
    out.open(output);

    sorted->SetCurrentPosition(0);

    while (sorted->GetCurrentPosition() != sorted->Size()) {
        out << sorted->ReadUInt32() << '\n';
        sorted->MoveRight();
    }
}

} // namespace NTape
