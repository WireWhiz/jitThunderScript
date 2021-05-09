# CMakeList.txt : CMake project for jitThunderScript, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("jitThunderScript")
set (CMAKE_CXX_STANDARD 17)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17")

# Add source to this project's executable.
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/bison)
include_directories(${PROJECT_SOURCE_DIR}/scripts)

add_executable (jitThunderScript "jitThunderScript.cpp" "jitThunderScript.h" "src/bison/bison.tab.cc" "src/bison/lex.yy.cc" "src/ThunderScriptCompiler.cpp" )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scripts
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})