cmake_minimum_required(VERSION 3.2.0)

# Choose Release mode as default.
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug Release"
    FORCE)
endif()

project(ssm LANGUAGES C CXX VERSION 0.1.0)
enable_testing()

set(ssm_definitions "")

add_subdirectory(extern)
add_subdirectory(source)
add_subdirectory(tests)
