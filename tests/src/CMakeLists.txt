include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(HEADER_FILES
        )

set(SOURCE_FILES
        test_token.cpp
        test_lexer.cpp)

add_executable(mint_test ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(mint_test gtest gtest_main)
target_link_libraries(mint_test src)
