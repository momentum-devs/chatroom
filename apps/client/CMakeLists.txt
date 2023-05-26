set(EXECUTABLE_NAME client)

set(SOURCES
        )

set(UT_SOURCES
        )

add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PUBLIC loguru::loguru messages utils)

target_include_directories(${EXECUTABLE_NAME} PUBLIC ${GMOCK_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR} ..)

#add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})
#
#target_link_libraries(${EXECUTABLE_NAME}UT PUBLIC gtest gmock_main)
#
#target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})
#
#add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})