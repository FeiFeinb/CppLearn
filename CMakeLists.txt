cmake_minimum_required(VERSION 3.19)
project(CppLearn)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppLearn src/main.cpp src/Utilities/AllHeader.h src/MyTuple.cpp src/STLLearn.cpp src/ConcurrentLearn.cpp src/Utilities/UtilityClasses.cpp src/Utilities/UtilityClasses.h src/Utilities/UtilityTemplate.hpp)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")