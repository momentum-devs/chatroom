set(LIBRARY_NAME collection)

set(SOURCES
        StringHelper.cpp
        )

set(UT_SOURCES
        StringHelperTest.cpp
        )

add_library(${LIBRARY_NAME} ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC)

target_link_libraries(${LIBRARY_NAME} PUBLIC)

add_executable(${LIBRARY_NAME}UT ${SOURCES} ${UT_SOURCES})

target_link_libraries(${LIBRARY_NAME}UT PRIVATE
        GTest::gtest_main
        )

target_include_directories(${LIBRARY_NAME}UT
        PRIVATE
        ${Boost_INCLUDE_DIR}
        ${GTEST_INCLUDE_DIR}
        )

add_test(NAME ${LIBRARY_NAME}UT COMMAND ${LIBRARY_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_code_coverage(${LIBRARY_NAME}UT ALL)
