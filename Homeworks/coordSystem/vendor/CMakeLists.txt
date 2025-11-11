add_subdirectory(json)

set(IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/misc/cpp/imgui_stdlib.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl3.h
)

set(IMPLOT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot.h
        ${CMAKE_CURRENT_SOURCE_DIR}/implot/implot_items.cpp
)

set(IMGUI_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)
set(IMPLOT_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/implot)

# Export to parent scope
set(IMGUI_SOURCES ${IMGUI_SOURCES} PARENT_SCOPE)
set(IMPLOT_SOURCES ${IMPLOT_SOURCES} PARENT_SCOPE)
set(IMGUI_INCLUDE_DIRS ${IMGUI_INCLUDE_DIRS} PARENT_SCOPE)
set(IMPLOT_INCLUDE_DIRS ${IMPLOT_INCLUDE_DIRS} PARENT_SCOPE)