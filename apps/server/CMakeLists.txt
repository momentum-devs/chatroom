set(EXECUTABLE_NAME server)

set(SOURCES
        api/SessionManager.cpp
        api/SessionImpl.cpp
        api/SessionFactoryImpl.cpp
        api/MessageHandlerImpl.cpp
        api/ConnectionAcceptorImpl.cpp
        application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImpl.cpp
        application/commandHandlers/deleteUserCommandHandler/DeleteUserCommandHandlerImpl.cpp
        application/services/hashService/HashServiceImpl.cpp
        application/services/tokenService/TokenServiceImpl.cpp
        config/EnvironmentParser.cpp
        config/ConfigProvider.cpp
        domain/entities/User.cpp
        infrastructure/database/management/DatabaseManagerFactory.cpp
        infrastructure/repositories/userRepository/UserRepositoryImpl.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImpl.cpp
        domain/entities/User.cpp
        application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerImpl.cpp
        )

set(UT_SOURCES
        api/MessageHandlerImplTest.cpp
        api/SessionImplTest.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImplTest.cpp
        api/SessionManagerTest.cpp
        )

set(IT_SOURCES
        infrastructure/repositories/userRepository/UserRepositoryImplIntegrationTest.cpp
        application/services/tokenService/TokenServiceImplIntegrationTest.cpp
        application/services/hashService/HashServiceImplIntegrationTest.cpp
        application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerImplIntegrationTest.cpp
        )

set(ODB_SOURCES
        core/database/driver.cpp
        core/database/database.h
        )

set(ODB_HEADERS
        core/database/person.h
        )

odb_compile(ODB_SOURCES FILES ${ODB_HEADERS} DB sqlite GENERATE_QUERY GENERATE_SESSION)

add_executable(${EXECUTABLE_NAME}
        Main.cpp
        ${SOURCES}
        ${ODB_SOURCES}
        ${ODB_HEADERS}
        )

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        Qt6::Core
        TinyOrm::TinyOrm
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        ${ODB_LIBRARIES}
        )

target_include_directories(${EXECUTABLE_NAME} PRIVATE
        ${ODB_INCLUDE_DIRS}
        ${ODB_COMPILE_OUTPUT_DIR}
        ${DOTENV_INCLUDE_DIR}
        ..
        )

target_compile_definitions(${EXECUTABLE_NAME}
        PRIVATE
        DATABASE_PGSQL
        )

add_subdirectory(infrastructure/database/management)

add_executable(${EXECUTABLE_NAME}UT ${UT_SOURCES} ${SOURCES})

target_link_libraries(${EXECUTABLE_NAME}UT PRIVATE
        gtest
        gmock_main
        Qt6::Core
        TinyOrm::TinyOrm
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        )

target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (INTEGRATION_TESTS_BUILD)
    add_executable(${EXECUTABLE_NAME}IT ${IT_SOURCES} ${SOURCES})

    target_link_libraries(${EXECUTABLE_NAME}IT PRIVATE
            gtest
            gmock_main
            Qt6::Core
            TinyOrm::TinyOrm
            loguru::loguru
            nlohmann_json::nlohmann_json
            cpp-jwt::cpp-jwt
            messages
            filesystem
            collection
            )

    target_include_directories(${EXECUTABLE_NAME}IT PUBLIC ${GMOCK_INCLUDE_DIR})

    add_test(NAME ${EXECUTABLE_NAME}IT COMMAND ${EXECUTABLE_NAME}IT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif ()
