find_package(GTest REQUIRED)

add_executable(tape_ut tape_ut.cpp)

target_link_libraries(
    tape_ut
    PRIVATE
    GTest::GTest
    tape-file
)

add_test(tape_gtests tape_ut)
