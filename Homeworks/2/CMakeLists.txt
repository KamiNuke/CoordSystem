cmake_minimum_required(VERSION 4.0)
project(lb2)

set(CMAKE_CXX_STANDARD 23)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(third-party/json)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
find_package(Boost 1.89 COMPONENTS REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(third-party/imgui)
include_directories(third-party/imgui/backends)

add_executable(lb2 main.cpp
        third-party/imgui/imconfig.h
        third-party/imgui/imgui.h
        third-party/imgui/imgui.cpp
        third-party/imgui/imgui_draw.cpp
        third-party/imgui/imgui_demo.cpp
        third-party/imgui/imgui_internal.h
        third-party/imgui/imgui_tables.cpp
        third-party/imgui/imgui_widgets.cpp
        third-party/imgui/imstb_rectpack.h
        third-party/imgui/imstb_textedit.h
        third-party/imgui/imstb_truetype.h
        third-party/imgui/misc/cpp/imgui_stdlib.cpp
        third-party/imgui/misc/cpp/imgui_stdlib.h
        third-party/imgui/backends/imgui_impl_opengl3.cpp
        third-party/imgui/backends/imgui_impl_opengl3.h
        third-party/imgui/backends/imgui_impl_opengl3_loader.h
        third-party/imgui/backends/imgui_impl_sdl3.cpp
        third-party/imgui/backends/imgui_impl_sdl3.h
        third-party/implot/implot.cpp
        third-party/implot/implot_internal.h
        third-party/implot/implot_demo.cpp
        third-party/implot/implot.h
        third-party/implot/implot_items.cpp
        WebSocketClient.cpp
        WebSocketClient.h
        CoordinateSystems.h)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARY} SDL3::SDL3 nlohmann_json::nlohmann_json)