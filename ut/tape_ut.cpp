#include <gtest/gtest.h>

#include <string>
#include <unistd.h>
#include <filesystem>
#include <fstream>
#include <random>
#include <ctime>

#include <tape-file.hpp>

namespace {

template <class I>
using TUniquePtr = std::unique_ptr<I, NBaseInterfaces::IBase::BaseDeleter>;



std::string GetCurrentDirectory() {
    return get_current_dir_name();
}

std::string PathDir;
std::mt19937 RandomGenerator(static_cast<uint32_t>(std::time(nullptr)));

std::string GenerateRandomPath() {
    std::string path = PathDir + "/";

    for (size_t i = 0; i < 20; i++) {
        char c = 'a' + RandomGenerator() % ('z' - 'a' + 1);
        path += c;
    }

    path += ".txt";
    return path;
}


TEST(TapeFileUt, CreateReadWrite) {
    std::string pathFile = GenerateRandomPath();
    std::ofstream out;
    out.open(pathFile);
    out << "10 20 30" << std::endl;

    TUniquePtr<NTape::ITape> tape(NTape::TTapeFile::Create(pathFile));

    std::vector<uint32_t> expected = {10, 20, 30, 20, 10, 40};
    std::vector<uint32_t> actual = {};

    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
    actual.push_back(tape->ReadUInt32());
    tape->MoveLeft();
    actual.push_back(tape->ReadUInt32());
    tape->MoveLeft();
    actual.push_back(tape->ReadUInt32());
    tape->WriteUInt32(40);
    actual.push_back(tape->ReadUInt32());

    ASSERT_EQ(expected, actual);
}

TEST(TapeFileUt, SetGetCurrentPosition) {
    std::string pathFile = GenerateRandomPath();
    std::ofstream out;
    out.open(pathFile);
    out << "10 20 30" << std::endl;

    TUniquePtr<NTape::ITape> tape(NTape::TTapeFile::Create(pathFile));

    ASSERT_EQ(tape->GetCurrentPosition(), static_cast<size_t>(0));
    tape->SetCurrentPosition(2);
    ASSERT_EQ(tape->GetCurrentPosition(), static_cast<size_t>(2));
    tape->SetCurrentPosition(1);
    ASSERT_EQ(tape->ReadUInt32(), static_cast<uint32_t>(20));
    tape->SetCurrentPosition(0);
    ASSERT_EQ(tape->ReadUInt32(), static_cast<uint32_t>(10));
    tape->SetCurrentPosition(2);
    ASSERT_EQ(tape->ReadUInt32(), static_cast<uint32_t>(30));
}

TEST(TapeFileUt, WriteMoveForce) {
    std::string pathFile = GenerateRandomPath();
    std::ofstream out;
    out.open(pathFile);

    TUniquePtr<NTape::ITape> tape(NTape::TTapeFile::Create(pathFile));

    tape->WriteUInt32(10, true);
    tape->MoveRight();
    tape->WriteUInt32(20, true);
    tape->MoveRight();
    tape->WriteUInt32(30, true);
    tape->MoveRight();
    tape->WriteUInt32(40, true);
    tape->MoveRight();
    tape->MoveRight(true);

    std::vector<uint32_t> expected = {10, 20, 30, 40, 0};
    std::vector<uint32_t> actual = {};
    tape->SetCurrentPosition(0);
    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
    actual.push_back(tape->ReadUInt32());
    tape->MoveRight();
}

TEST(TapeSort, SortSmall) {
    std::string inFile = GenerateRandomPath();
    std::ofstream in;
    in.open(inFile);
    in << "16 5 2 1 3 2 17" << std::endl;
    in.close();

    std::string outFile = GenerateRandomPath();

    NTape::Sort(inFile, outFile, 1024);

    std::vector<uint32_t> expected = {1, 2, 2, 3, 5, 16, 17};
    std::vector<uint32_t> actual = {};

    std::ifstream out;
    out.open(outFile);

    uint32_t x;
    while (out >> x) {
        actual.push_back(x);
    }

    ASSERT_EQ(expected, actual);
}

TEST(TapeSort, SortWithNotDefaultOption) {
    std::string inFile = GenerateRandomPath();
    std::ofstream in;
    in.open(inFile);
    in << "16 5 2 1 3 2 17 0 1 3 4" << std::endl;
    in.close();

    std::string outFile = GenerateRandomPath();

    NTape::Sort(inFile, outFile, 4);

    std::vector<uint32_t> expected = {0, 1, 1, 2, 2, 3, 3, 4, 5, 16, 17};
    std::vector<uint32_t> actual = {};

    std::ifstream out;
    out.open(outFile);

    uint32_t x;
    while (out >> x) {
        actual.push_back(x);
    }

    ASSERT_EQ(expected, actual);
}

}; // namespace

void Prepare() {
    PathDir = GetCurrentDirectory() + "/tmp";
    std::filesystem::create_directory(PathDir);
}

void Complete() {
    std::filesystem::remove_all(PathDir);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    Prepare();
    auto result = RUN_ALL_TESTS();
    Complete();
    return result;
}
