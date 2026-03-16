cmake_minimum_required(VERSION 4.1)
project(ngc)

set(CMAKE_CXX_STANDARD 20)

add_executable(ngc src/main.cpp
        src/lexer.cpp
        src/lexer.h
        src/ast_node.cpp
        src/ast_node.h
        src/parser.cpp
        src/parser.h
        src/backends/backend.cpp
        src/backends/backend.h
        src/backends/llvm.cpp
        src/backends/llvm.h)

find_package(LLVM 18.1.8 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

target_include_directories(ngc PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(ngc PRIVATE ${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(LLVM_LIBS
        core
        support
        irreader
        x86codegen
        x86asmparser
        passes
)

target_link_libraries(ngc PRIVATE ${LLVM_LIBS})