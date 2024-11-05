project(SALT_PROJECT)
cmake_minimum_required(VERSION 3.5.1)

# CMake Options
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER}) # use g++ for .c files (flute)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SALT_SRCS salt/*.cpp salt/*/*.cpp salt/base/flute/*.c)
file(GLOB OTHER_METHODS_SRCS other_methods/*.cpp)
add_library(salt STATIC ${SALT_SRCS})
add_library(other_methods OBJECT ${OTHER_METHODS_SRCS})

# unittest_salt
add_executable(unittest_salt unittest/unittest_main.cpp $<TARGET_OBJECTS:other_methods>)
target_link_libraries(unittest_salt salt)

# minimal_salt
add_executable(minimal_salt examples/minimal_main.cpp)
target_link_libraries(minimal_salt salt)

# eval_single_salt
add_executable(eval_single_salt examples/eval_single_main.cpp $<TARGET_OBJECTS:other_methods>)
target_link_libraries(eval_single_salt salt)

# eval_batch_salt
add_executable(eval_batch_salt examples/eval_batch_main.cpp $<TARGET_OBJECTS:other_methods>)
target_link_libraries(eval_batch_salt salt)

# python binding
execute_process(
    COMMAND python3
    -m pybind11
    --cmakedir
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "pybind11 cmakedir: ${pybind11_DIR}")
find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(pysaltrouting NO_EXTRAS examples/pysaltrouting.cpp)
target_link_libraries(pysaltrouting PRIVATE salt)
set_target_properties(pysaltrouting PROPERTIES PREFIX "" SUFFIX ".so")