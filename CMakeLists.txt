cmake_minimum_required(VERSION 2.6)
project(SerializingTests)

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

enable_testing()

find_package(Boost REQUIRED)

include_directories(include)

add_subdirectory(test build)