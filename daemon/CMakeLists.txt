cmake_minimum_required(VERSION 3.20)

project(sysobserverd)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("include")

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(
    sysobserverd
    ${SOURCES}
)