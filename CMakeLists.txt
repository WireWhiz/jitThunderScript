# CMakeList.txt : CMake project for jitThunderScript, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("jitThunderScript" VERSION 0.0.0)
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES src/*.h src/*.cpp src/*.hh src/*.cc)
add_compile_definitions(ASMJIT_STATIC)

include_directories(
    src/bison
    src/libjit
)

if (UNIX)
    add_link_options(-lrt)
endif (UNIX)

add_executable (jitThunderScript "jitThunderScript.cpp"  ${SOURCES})

target_include_directories(jitThunderScript PRIVATE ${YOUR_DIRECTORY}/src/bison)
target_include_directories(jitThunderScript PRIVATE ${YOUR_DIRECTORY}/src/asmjit)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scripts
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})