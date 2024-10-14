#include <gtest/gtest.h>

#include <string>
#include <unistd.h>
#include <filesystem>
#include <fstream>

#include <tape-file.hpp>

std::string GetCurrentDirectory() {
    return get_current_dir_name();
}

template <class I>
using TUniquePtr = std::unique_ptr<I, NBaseInterfaces::IBase::BaseDeleter>;

TEST(TapeFileUt, CreateReadWrite) {
    std::string pathDir = GetCurrentDirectory() + "/tmp";
    std::filesystem::create_directory(pathDir);
    std::string pathFile = pathDir + "/a.txt";
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

    // todo smth with it
    std::filesystem::remove_all(pathDir);
}

TEST(TapeFileUt, SetGetCurrentPosition) {
    std::string pathDir = GetCurrentDirectory() + "/tmp";
    std::filesystem::create_directory(pathDir);
    std::string pathFile = pathDir + "/a.txt";
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

    std::filesystem::remove_all(pathDir);
}

TEST(TapeFileUt, WriteMoveForce) {
    std::string pathDir = GetCurrentDirectory() + "/tmp";
    std::filesystem::create_directory(pathDir);
    std::string pathFile = pathDir + "/a.txt";
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

    std::filesystem::remove_all(pathDir);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
