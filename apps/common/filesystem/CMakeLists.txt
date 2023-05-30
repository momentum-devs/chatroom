set(LIBRARY_NAME filesystem)

set(SOURCES
        FileSystemServiceImpl.cpp
        GetProjectPath.cpp
        GetExecutablePath.cpp
        FileSystemServiceFactory.cpp
        )

set(UT_SOURCES
        FileSystemServiceImplTest.cpp
        GetExecutablePathTest.cpp
        GetProjectPathTest.cpp
        )

add_library(${LIBRARY_NAME} ${SOURCES})

get_filename_component(PARENT_DIR ../ ABSOLUTE)

target_include_directories(${LIBRARY_NAME} PUBLIC ${PARENT_DIR})

target_link_libraries(${LIBRARY_NAME} PUBLIC collection)

add_executable(${LIBRARY_NAME}UT ${SOURCES} ${UT_SOURCES})

target_link_libraries(${LIBRARY_NAME}UT PRIVATE gtest_main collection)

target_include_directories(${LIBRARY_NAME}UT
        PRIVATE
        ${Boost_INCLUDE_DIR}
        ${GTEST_INCLUDE_DIR}
        ${PARENT_DIR}
        )

add_test(NAME ${LIBRARY_NAME}UT COMMAND ${LIBRARY_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} --gtest_color=yes)