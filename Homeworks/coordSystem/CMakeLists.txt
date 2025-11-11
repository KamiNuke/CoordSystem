cmake_minimum_required(VERSION 4.0)
project(coordSystem)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(Boost 1.89 COMPONENTS REQUIRED)

add_subdirectory(vendor)
add_subdirectory(Core)
add_subdirectory(App)