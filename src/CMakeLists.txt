cmake_minimum_required (VERSION 3.16.0)
project ("amazons")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(core)

add_executable(amazons main.cpp)


target_link_libraries(amazons PUBLIC amazons-core)
target_include_directories(amazons PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/core"
                           )



