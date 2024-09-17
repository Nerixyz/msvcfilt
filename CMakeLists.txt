cmake_minimum_required(VERSION 3.20)

project(msvcfilt VERSION 0.1.0 LANGUAGES CXX)

add_executable(msvcfilt msvcfilt.cpp)
set_target_properties(msvcfilt PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED On
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
target_link_libraries(msvcfilt PRIVATE Dbghelp.lib)
