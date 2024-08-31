cmake_minimum_required (VERSION 3.22)


if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

project ("NeuralNetworkProject")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB NEURON_SOURCE_FILES "NeuralNetwork/src/Core/Neuron/*.cpp")
file(GLOB NEURON_HEADER_FILES "NeuralNetwork/src/Core/Neuron/*.h")
file(GLOB LAYER_SOURCE_FILES "NeuralNetwork/src/Core/Layer/*.cpp")
file(GLOB LAYER_HEADER_FILES "NeuralNetwork/src/Core/Layer/*.h")
file(GLOB UTILS_SOURCE_FILES "NeuralNetwork/src/Core/Utils/*.cpp")
file(GLOB UTILS_HEADER_FILES "NeuralNetwork/src/Core/Utils/*.h")
file(GLOB NEURONACTIVATION_SOURCE_FILES "NeuralNetwork/src/Core/NeuronActivation/*.cpp")
file(GLOB NEURONACTIVATION_HEADER_FILES "NeuralNetwork/src/Core/NeuronActivation/*.h")

add_subdirectory("vendor/fmt")

add_executable(NeuralNetwork
    "NeuralNetwork/src/main.cpp"
    "NeuralNetwork/src/Core/NeuralNetwork.h"
    "NeuralNetwork/src/Core/NeuralNetwork.cpp"
    ${NEURON_SOURCE_FILES}
    ${NEURON_HEADER_FILES}
    ${LAYER_SOURCE_FILES}
    ${LAYER_HEADER_FILES}
    ${UTILS_SOURCE_FILES}
    ${UTILS_HEADER_FILES}
    ${NEURONACTIVATION_SOURCE_FILES}
    ${NEURONACTIVATION_HEADER_FILES}
 )
    
set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(NN_ENABLE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NN_ENABLE_RELEASE)
endif()

target_precompile_headers(NeuralNetwork PUBLIC "NeuralNetwork/src/nnpch.h")

target_include_directories(NeuralNetwork PUBLIC "NeuralNetwork/src")
target_include_directories(NeuralNetwork PUBLIC "vendor/fmt/include")

target_link_libraries(NeuralNetwork PRIVATE fmt::fmt fmt-header-only)

set_target_properties(NeuralNetwork PROPERTIES OUTPUT_NAME "NeuralNetwork")

