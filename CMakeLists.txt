cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME chatroom)
project(${PROJECT_NAME} CXX)

option(INTEGRATION_TESTS_BUILD "build and run integration tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

find_package(ODB REQUIRED COMPONENTS qt pgsql OPTIONAL_COMPONENTS mysql sqlite)
include(${ODB_USE_FILE})

find_package(Boost REQUIRED)
find_package(Qt6 6.2 COMPONENTS Core Gui Widgets Quick QuickControls2 LinguistTools REQUIRED)
find_package(TinyOrm 0.16.0 CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(cpp-jwt REQUIRED)

add_subdirectory(externals)

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(DOTENV_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/dotenv-cpp/include")
set(TOM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/TinyORM/tom/include")

enable_testing()
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" OFF)
if (${ENABLE_COVERAGE})
    add_compile_options(--coverage -O0)
    link_libraries(--coverage)
endif ()

add_subdirectory(apps)
