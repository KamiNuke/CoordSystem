set(SOURCES
        Source/main.cpp
)

add_executable(${PROJECT_NAME}
        Source/LB1/LB1.cpp
        Source/LB1/LB1.h
        Source/LB2/LB2.cpp
        Source/LB2/LB2.h
        Source/LB3/LB3.cpp
        Source/LB3/LB3.h)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})

target_link_libraries(${PROJECT_NAME} Core)

target_include_directories(${PROJECT_NAME} PRIVATE Source)