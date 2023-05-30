set(LIBRARY_NAME messages)

set(SOURCES
        MessageSerializerImpl.cpp
        MessageReaderImpl.cpp
        MessageSenderImpl.cpp
        )

set(UT_SOURCES
        MessageSerializerImplTest.cpp
        )

add_library(${LIBRARY_NAME} ${SOURCES})

target_link_libraries(${LIBRARY_NAME} PUBLIC
        loguru::loguru)

target_include_directories(${LIBRARY_NAME} PUBLIC ../..)

add_executable(${LIBRARY_NAME}UT ${UT_SOURCES} ${SOURCES})

target_link_libraries(${LIBRARY_NAME}UT PUBLIC gtest gmock_main)

target_include_directories(${LIBRARY_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR} ../..)

add_test(NAME ${LIBRARY_NAME}UT COMMAND ${LIBRARY_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
