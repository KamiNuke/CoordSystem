set(SOURCES
        Source/CoordinateSystems.h
        Source/WebSocketClient.cpp
        Source/WebSocketClient.h
        Source/Core/Application.cpp
        Source/Core/Application.h
        Source/Core/Window.cpp
        Source/Core/Window.h
        Source/Core/Layer.h
        Source/ImGui/ImGuiLayer.cpp
        Source/ImGui/ImGuiLayer.h
        ${IMGUI_SOURCES}
        ${IMPLOT_SOURCES}
)

add_library(Core STATIC)
target_sources(Core PRIVATE ${SOURCES})

target_link_libraries(Core SDL3::SDL3)
target_link_libraries(Core nlohmann_json::nlohmann_json)
target_link_libraries(Core ${OPENGL_LIBRARY})

target_include_directories(Core
        PUBLIC
            Source
            ${IMGUI_INCLUDE_DIRS}
            ${IMPLOT_INCLUDE_DIRS}
        PRIVATE
            ${Boost_INCLUDE_DIRS}
)