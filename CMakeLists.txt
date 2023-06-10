cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME chatroom)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
-
find_package(TinyOrm 0.16.0 CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_subdirectory(externals)

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(DOTENV_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/dotenv-cpp/include")
set(TOM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/TinyORM/tom/include")

enable_testing()

add_subdirectory(apps)
