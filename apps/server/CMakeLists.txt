set(EXECUTABLE_NAME server)

set(SOURCES
        api/SessionManager.cpp
        api/SessionImpl.cpp
        )

set(UT_SOURCES
        )

add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        TinyOrm::TinyOrm
        loguru::loguru
        messages
        utils)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${GMOCK_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR} ..)

#add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})
#
#target_link_libraries(${EXECUTABLE_NAME}UT PUBLIC gtest gmock_main taopq)
#
#target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})
#
#add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
