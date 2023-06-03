set(EXECUTABLE_NAME server)

set(SOURCES
        api/SessionManager.cpp
        api/SessionImpl.cpp
        infrastructure/database/management/DatabaseManagerFactory.cpp
        infrastructure/repositories/userRepository/UserRepositoryImpl.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImpl.cpp
        domain/entities/User.cpp
        application/commandHandlers/CreateUserCommandHandlerImpl.cpp
        )

set(UT_SOURCES
        )

set(IT_SOURCES
        )

add_executable(${EXECUTABLE_NAME} Main.cpp ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        Qt6::Core
        TinyOrm::TinyOrm
        loguru::loguru
        messages
        filesystem
        environment
        collection
        )

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${GMOCK_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR} ..)

add_subdirectory(infrastructure/database/management)

#add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})
#
#target_link_libraries(${EXECUTABLE_NAME}UT PUBLIC gtest gmock_main taopq)
#
#target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})
#
#add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(${EXECUTABLE_NAME}IT ${IT_SOURCES} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME}IT PRIVATE
        gtest
        gmock_main
        Qt6::Core
        TinyOrm::TinyOrm
        loguru::loguru
        messages
        filesystem
        environment
        collection
        )

target_include_directories(${EXECUTABLE_NAME}IT PUBLIC ${GMOCK_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}IT COMMAND ${EXECUTABLE_NAME}IT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
