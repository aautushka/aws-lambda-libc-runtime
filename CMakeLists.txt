cmake_minimum_required(VERSION 3.5)
project(test LANGUAGES C)
include_directories(src)
add_executable(${PROJECT_NAME} src/runtime.c tests/function.c)

