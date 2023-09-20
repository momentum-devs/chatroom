cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME chatroom)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++20 -Wall -Wextra -Wno-unknown-pragmas")

find_package(Boost REQUIRED)
find_package(Qt6 6.2 COMPONENTS Core Gui Widgets Quick QuickControls2 LinguistTools REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(cpp-jwt REQUIRED)
find_package(GTest REQUIRED)

add_subdirectory(externals)

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(DOTENV_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/dotenv-cpp/include")
set(FAKER_CXX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/faker-cxx/include")

enable_testing()

set(target_code_coverage_ALL 1)
include("cmake/cmake-coverage.cmake")
add_code_coverage_all_targets()

add_subdirectory(apps)
