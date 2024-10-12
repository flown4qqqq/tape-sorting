#include "tape-file.hpp"
#include <random>
#include <ctime>
#include <stdexcept>

namespace NTape {

std::string TTapeFile::normilize(uint32_t n) {
    auto result = std::to_string(n);
    while (result.size() < normalizeCoefficient) {
        result = "0" + result;
    }

    return result;
}

TTapeFile::~TTapeFile() {
    fclose(file);
}

TTapeFile::TTapeFile()
    : pos(0)
    , size(0)
{
    std::mt19937 mt(std::time(nullptr));
    std::string path = "/tmp/tape-sort-";

    for (size_t i = 0; i < 20; i++) {
        char c = 'a' + mt() % ('z' - 'a' + 1);
        path += c;
    }

    this->file = fopen(path.c_str(), "w+");
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
    for (char c = fgetc(from); ; c = fgetc(from)) {
        if (isDigit(c)) {
            num *= 10;
            num += (c - '0');
            empty = false;
        } else {
            if (!empty) {
                empty = true;
                auto normNum = normilize(num);
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
}

void TTapeFile::Delete() {
    delete this;
}

bool TTapeFile::MoveLeft() {
    if (this->pos == 0) {
        return false;
    }

    this->pos--;
    fseek(file, -(normalizeCoefficient + 1), SEEK_CUR);
    return true;
}

bool TTapeFile::MoveRight() {
    if (this->pos == this->size) {
        return false;
    }

    this->pos++;
    fseek(file, normalizeCoefficient + 1, SEEK_CUR);
    return true;
}

uint32_t TTapeFile::ReadUInt32() const {
    if (this->pos == this->size) {
        throw new std::out_of_range("TTapeFile::ReadUInt32: pos equals size");
    }

    std::string num;
    num.reserve(normalizeCoefficient);

    for (size_t i = 0; i < num.size(); i++) {
        num.push_back(fgetc(this->file));
    }

    fseek(file, -normalizeCoefficient, SEEK_CUR);

    return std::stoul(num);
}

void TTapeFile::WriteUInt32(uint32_t num) {
    std::string normNum = normilize(num);

    for (size_t i = 0; i < normNum.size(); i++) {
        fputc(normNum[i], this->file);
    }

    fseek(file, -normalizeCoefficient, SEEK_CUR);
}

} // namespace NTape
