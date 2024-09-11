cmake_minimum_required(VERSION 3.22)

project(NNVisualizer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(DIRECTX_SDK_DIR NAMES d3d11.h PATH_SUFFIXES DirectXSDK)
if (DIRECTX_SDK_DIR)
    message(STATUS "Found DirectX SDK at ${DIRECTX_SDK_DIR}")
else()
    message(FATAL_ERROR "DirectX SDK not found")
endif()

include_directories(${DIRECTX_SDK_DIR}/Include)
link_directories(${DIRECTX_SDK_DIR}/Lib/x64)

# Include directories for the NeuralNetwork project
include_directories(${CMAKE_SOURCE_DIR}/NNVisualizer/src)
include_directories(${CMAKE_SOURCE_DIR}/NeuralNetwork/src)
include_directories(${CMAKE_SOURCE_DIR}/NeuralNetwork/vendor/fmt/include)

file(GLOB VISUALIZER_SOURCE_FILES "src/*.cpp")

add_executable(NNVisualizer
    src/main.cpp
    ${VISUALIZER_SOURCE_FILES}
 "src/NNVisualizer/Visualizer.h" "src/NNVisualizer/Visualizer.cpp" "src/NNVisualizer/Utils/DirectxUtilities.h"  "src/NNVisualizer/Renderer/Camera.h" "src/NNVisualizer/Renderer/Camera.cpp" "src/NNVisualizer/Components/Button.h" "src/NNVisualizer/Components/Button.cpp" "src/NNVisualizer/Utils/Utils.h" "src/NNVisualizer/Utils/LoopState.h" "src/NNVisualizer/Components/TextField.h" "src/NNVisualizer/Components/TextField.cpp" "src/NNVisualizer/Components/Components.h")

# Link against NeuralNetwork and other necessary libraries
target_link_libraries(NNVisualizer
    NeuralNetwork
    d3d11
    d2d1
    dxgi
    dwrite
)

# Specify output directory for executable
set_target_properties(NNVisualizer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Precompiled headers setup
target_precompile_headers(NNVisualizer PUBLIC "src/nnvpch.h")

# Add a post-build command to copy the DLL to the executable directory
add_custom_command(TARGET NNVisualizer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:NeuralNetwork>
    $<TARGET_FILE_DIR:NNVisualizer>
)
