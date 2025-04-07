cmake_minimum_required(VERSION 3.22)

project(NeuralNetwork)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB NEURON_SOURCE_FILES "src/Core/Neuron/*.cpp")
file(GLOB LAYER_SOURCE_FILES "src/Core/Layer/*.cpp")
file(GLOB UTILS_SOURCE_FILES "src/Core/Utils/*.cpp")
file(GLOB NEURONACTIVATION_SOURCE_FILES "src/Core/NeuronActivation/*.cpp")

# Add external libraries
add_subdirectory(vendor/fmt)

# Define the shared library
add_library(NeuralNetwork SHARED
    src/Core/NeuralNetwork.cpp
    ${NEURON_SOURCE_FILES}
    ${LAYER_SOURCE_FILES}
    ${UTILS_SOURCE_FILES}
    ${NEURONACTIVATION_SOURCE_FILES}
    "src/Core/Utils/Exceptions.h"
)

# Set the export macro for DLL
target_compile_definitions(NeuralNetwork PRIVATE BUILDING_NEURALNETWORK)


# Include directories
target_include_directories(NeuralNetwork PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/fmt/include
)

# Link external libraries
target_link_libraries(NeuralNetwork PUBLIC fmt::fmt)

# Set output properties for the shared library
set_target_properties(NeuralNetwork PROPERTIES OUTPUT_NAME "NeuralNetwork")

# Specify output directory for DLL
set_target_properties(NeuralNetwork PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Precompiled headers setup
target_precompile_headers(NeuralNetwork PUBLIC "src/nnpch.h")

# Conditional compilation flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(NeuralNetwork PRIVATE NN_ENABLE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(NeuralNetwork PRIVATE NN_ENABLE_RELEASE)
endif()
