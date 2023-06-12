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

set(odbcrs_headers
        src/Branch.hpp
        src/Employee.hpp
        src/Person.hpp
        src/Customer.hpp
        src/Database.hpp
        src/Date.hpp
        src/Address.hpp
        src/LicenseClass.hpp
        )

odb_compile(
        HEADERS ${odbcrs_headers}
        DB pgsql
        STANDARD c++11
        GENERATE_QUERY GENERATE_SESSION GENERATE_SCHEMA
        HEADER_SUFFIX ".hpp"
        INLINE_SUFFIX ".ipp"
        SOURCE_SUFFIX ".cpp"
        FILE_SUFFIX ".odb"
        DEFAULT_POINTER "std::shared_ptr"
)

set(odbcrs_sources
        src/Branch.cpp
        src/Employee.cpp
        src/Person.cpp
        src/Customer.cpp
        src/Database.cpp
        src/Date.cpp
        src/Address.cpp
        src/LicenseClass.cpp
        ${ODB_GENERATED_SOURCES}
        )

print_var(ODB_GENERATED_SOURCES)

set(odbcrs_libraries
        odb
        odb-pgsql
        )


include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -Wno-unknown-pragmas")

add_executable(${EXECUTABLE_NAME}
        Main.cpp
        ${SOURCES}
        ${odbcrs_sources}
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
        ..
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
