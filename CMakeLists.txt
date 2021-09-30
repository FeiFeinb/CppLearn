cmake_minimum_required(VERSION 3.19)
project(CppLearn)

set(CMAKE_CXX_STANDARD 20)

add_executable(CppLearn main.cpp AllHeader.h MyTuple.cpp STLLearn.cpp ConcurrentLearn.cpp CppUtilities.cpp CppUtilities.h)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")