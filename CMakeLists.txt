cmake_minimum_required(VERSION 4.3.3)

include(FetchContent)
project(gdsense)

FetchContent_Declare(godot-cpp
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp
    GIT_TAG godot-4.5-stable
)

FetchContent_MakeAvailable(godot-cpp)

FetchContent_Declare(dualsense-multiplatform
    GIT_REPOSITORY https://github.com/rafaelvaloto/Dualsense-Multiplatform
    GIT_TAG v1.0.5
)

FetchContent_MakeAvailable(dualsense-multiplatform)

add_library(${PROJECT_NAME} SHARED
    include/sony_controller_manager.hpp
    src/init.cpp
    src/sony_controller_manager.cpp
    src/godot_registry_policy.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_include_directories(${PROJECT_NAME} PRIVATE ${dualsense-multiplatform_SOURCE_DIR}/Tests/Common)
target_include_directories(${PROJECT_NAME} PRIVATE ${godot-cpp_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${godot-cpp_SOURCE_DIR}/gdextension)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/_deps/godot-cpp-build/gen/include)

configure_file(${CMAKE_SOURCE_DIR}/gdsense.gdextension ${CMAKE_CURRENT_BINARY_DIR}/gdense.gdextension COPYONLY)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GamepadCoreTestCommon
    godot::cpp
)