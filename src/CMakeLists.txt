cmake_minimum_required(VERSION 3.15...3.26)
project(
  "${SKBUILD_PROJECT_NAME}"
  LANGUAGES CXX
  VERSION "${SKBUILD_PROJECT_VERSION}")

find_package(
  Python
  COMPONENTS Interpreter Development.Module
  REQUIRED)

find_package(pybind11 CONFIG REQUIRED)
pybind11_add_module(_amazons main.cpp core.cpp)

target_compile_definitions(_amazons
                           PRIVATE VERSION_INFO=${PROJECT_VERSION})

install(TARGETS _amazons LIBRARY DESTINATION .)