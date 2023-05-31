cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME chatroom)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

if(WSL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -stdlib=libc++ -fexperimental-library")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -std=c++20 -stdlib=libc++ -lc++abi")
endif(WSL)


find_package(Boost REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets Gui REQUIRED)
find_package(TinyOrm 0.16.0 CONFIG REQUIRED)

add_subdirectory(externals)

set(GMOCK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googlemock/include")
set(GTEST_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/googletest/googletest/include")
set(DOTENV_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/dotenv-cpp/include")
set(TOM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/externals/TinyORM/tom/include")

enable_testing()

add_subdirectory(apps)
