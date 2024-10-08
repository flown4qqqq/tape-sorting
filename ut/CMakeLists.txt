find_package(GTest REQUIRED)

add_executable(addition_ut addition_ut.cpp)

target_link_libraries(
    addition_ut
    PRIVATE
    GTest::GTest
    foo
)

add_test(addition_gtests addition_ut)
