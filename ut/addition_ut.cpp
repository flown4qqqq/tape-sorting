#include <foo.hpp>
#include <gtest/gtest.h>

TEST(groupName, firstTest) {
    const auto expected = 4;
    const auto actual = TAdd::addition(2, 2);
    ASSERT_EQ(expected, actual);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
