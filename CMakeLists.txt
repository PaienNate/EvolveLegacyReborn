cmake_minimum_required(VERSION 3.21)

project(EvolveLegacyRebornServer LANGUAGES CXX)

option(EVOLVE_BUILD_TESTING "Build server logic tests" ON)
option(EVOLVE_BUILD_RUNTIME "Build the embeddable HTTPS server runtime" OFF)
option(EVOLVE_BUILD_HARNESS "Build the standalone server harness" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(CTest)
add_subdirectory(server_core)
