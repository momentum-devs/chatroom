set(LIBRARY_NAME utils)

set(SOURCES
        environmentParser/EnvironmentParser.cpp
        fileSystem/FileSystemServiceImpl.cpp
        fileSystem/GetProjectPath.cpp
        fileSystem/GetExecutablePath.cpp
        fileSystem/FileSystemServiceFactory.cpp
        collection/StringHelper.cpp
        )

set(UT_SOURCES
        fileSystem/FileSystemServiceImplTest.cpp
        fileSystem/GetExecutablePathTest.cpp
        fileSystem/GetProjectPathTest.cpp
        collection/StringHelperTest.cpp
        )

add_library(${LIBRARY_NAME} ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ../.. .)

target_link_libraries(${LIBRARY_NAME} PUBLIC)

add_executable(${LIBRARY_NAME}UT ${SOURCES} ${UT_SOURCES})

target_link_libraries(${LIBRARY_NAME}UT PRIVATE gtest_main)

target_include_directories(${LIBRARY_NAME}UT
        PRIVATE
        "include/fileSystem"
        ${Boost_INCLUDE_DIR}
        ${GTEST_INCLUDE_DIR}
        )

add_test(${LIBRARY_NAME}UT ${LIBRARY_NAME}UT --gtest_color=yes)