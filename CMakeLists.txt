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

find_path(HIREDIS_HEADER hiredis)
target_include_directories(target PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(target ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(target PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(target ${REDIS_PLUS_PLUS_LIB})

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(DOTENV_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/dotenv-cpp/include")

enable_testing()

set(target_code_coverage_ALL 1)
include("cmake/cmake-coverage.cmake")
add_code_coverage_all_targets()

add_subdirectory(apps)
