include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(mint_test mint_test.h mint_test.cpp)

target_link_libraries(mint_test gtest gtest_main)
target_link_libraries(mint_test src)
