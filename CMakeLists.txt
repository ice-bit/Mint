cmake_minimum_required(VERSION 3.12)
project(mint)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -Werror -pedantic-errors -fstack-protector-all)
endif()

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp)

add_executable(mint ${SOURCE_FILES})

include_directories(src)
add_subdirectory(src)
add_subdirectory(tests)


target_link_libraries(mint src)
