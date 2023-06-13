set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(ODB)

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

set(ODB_HEADERS
        infrastructure/database/tables/userTable/User.h
        )

odb_compile(
        HEADERS ${ODB_HEADERS}
        DB pgsql
        STANDARD c++14
        GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA
        HEADER_SUFFIX ".h"
        INLINE_SUFFIX ".i"
        SOURCE_SUFFIX ".cpp"
        FILE_SUFFIX ".odb"
        DEFAULT_POINTER "std::shared_ptr"
)

set(ODB_SOURCES
        infrastructure/database/tables/userTable/User.cpp
        ${ODB_GENERATED_SOURCES}
        )

set(odbcrs_libraries
        odb
        odb-pgsql
        )


include_directories(infrastructure/database/tables/userTable)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-unknown-pragmas")

add_executable(${EXECUTABLE_NAME}
        Main.cpp
        ${SOURCES}
        ${ODB_SOURCES}
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
        ${odbcrs_libraries}
        )

target_include_directories(${EXECUTABLE_NAME} PRIVATE
        ${DOTENV_INCLUDE_DIR}
        )

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
