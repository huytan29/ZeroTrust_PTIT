cmake_minimum_required(VERSION 3.20)

project(ztas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ztas main.cpp)

# Trên macOS/Homebrew: dùng CMake target chính thức.
find_package(httplib CONFIG QUIET)

if(TARGET httplib::httplib)
    target_link_libraries(ztas PRIVATE httplib::httplib)
else()
    # Trong Debian Docker: tìm header và thư viện cài bởi apt.
    find_path(HTTPLIB_INCLUDE_DIR NAMES httplib.h REQUIRED)
    find_library(HTTPLIB_LIBRARY NAMES cpp-httplib httplib REQUIRED)

    target_include_directories(ztas PRIVATE ${HTTPLIB_INCLUDE_DIR})
    target_link_libraries(ztas PRIVATE ${HTTPLIB_LIBRARY})
endif()