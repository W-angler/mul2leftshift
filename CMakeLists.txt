CMAKE_MINIMUM_REQUIRED(VERSION 3.1)
PROJECT(Mul2LeftShiftPass)

#set your LLVM installation directory
set(LLVM_DIR ..)
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_library(Mul2LeftShiftPass MODULE Mul2LeftShiftPass.cpp)

target_compile_features(Mul2LeftShiftPass PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(Mul2LeftShiftPass PROPERTIES
    COMPILE_FLAGS "-fno-rtti"
)
