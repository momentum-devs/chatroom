cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME chatroom)
project(${PROJECT_NAME} CXX)

find_package(Boost REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)
find_package(TinyOrm 0.16.0 CONFIG REQUIRED)

add_subdirectory(externals)

set(CMAKE_CXX_STANDARD 20)

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(DOTENV_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/dotenv-cpp/include")

enable_testing()

add_subdirectory(apps)
