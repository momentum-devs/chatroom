set(EXECUTABLE_NAME server)

set(SOURCES
        ../common/EnvironmentParser.cpp
        )

set(UT_SOURCES
        )

add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        loguru::loguru
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Charts
        TinyOrm::TinyOrm
        )

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${GMOCK_INCLUDE_DIR})

#add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})
#
#target_link_libraries(${EXECUTABLE_NAME}UT PUBLIC fmt gtest gmock_main taopq)
#
#target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})
#
#add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
