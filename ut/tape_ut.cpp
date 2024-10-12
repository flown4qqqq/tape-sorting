#include <gtest/gtest.h>

#include <string>
#include <unistd.h>
#include <filesystem>
#include <fstream>

#include <tape-file.hpp>

std::string GetCurrentDirectory() {
    const int pathMax = 256;
    char result[pathMax];
    auto count = readlink("/proc/self/exe", result, pathMax);
    
    if (count > 0) {
        result[count] = '\0';
        while (result[count] != '/') {
            result[count] = '\0';
            count--;
        }
    }

    return std::string(result, static_cast<uint32_t>((count > 0) ? count : 0));
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

    std::filesystem::remove_all(pathDir);

    ASSERT_EQ(expected, actual);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
