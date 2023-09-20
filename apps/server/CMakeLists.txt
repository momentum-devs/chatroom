set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(ODB)

set(EXECUTABLE_NAME server)

set(SOURCES
        api/SessionManager.cpp
        api/SessionImpl.cpp
        api/SessionFactoryImpl.cpp
        api/ConnectionAcceptorImpl.cpp
        api/MessageRouterImpl.cpp
        api/MessageRouterFactory.cpp
        api/MessageHandlers/RegisterMessageHandler.cpp
        api/MessageHandlers/LoginMessageHandler.cpp
        api/MessageHandlers/CreateChannelMessageHandler.cpp
        api/MessageHandlers/GetUserChannelsMessageHandler.cpp
        api/MessageHandlers/GetUserDataMessageHandler.cpp
        config/EnvironmentParser.cpp
        config/ConfigProvider.cpp
        core/database/DatabaseConnectionFactory.cpp

        domain/entities/user/User.cpp
        infrastructure/repositories/userRepository/UserRepositoryImpl.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImpl.cpp
        application/services/hashService/HashServiceImpl.cpp
        application/services/tokenService/TokenServiceImpl.cpp
        application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImpl.cpp
        application/commandHandlers/deleteUserCommandHandler/DeleteUserCommandHandlerImpl.cpp
        application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerImpl.cpp
        application/commandHandlers/updateUserCommandHandler/UpdateUserCommandHandlerImpl.cpp
        application/commandHandlers/logoutUserCommandHandler/LogoutUserCommandHandlerImpl.cpp
        application/commandHandlers/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImpl.cpp
        application/commandHandlers/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImpl.cpp
        application/queryHandlers/findUserQueryHandler/FindUserQueryHandlerImpl.cpp
        infrastructure/services/emailService/EmailServiceImpl.cpp

        domain/entities/channel/Channel.cpp
        infrastructure/repositories/channelRepository/ChannelRepositoryImpl.cpp
        infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.cpp
        application/commandHandlers/createChannelCommandHandler/CreateChannelCommandHandlerImpl.cpp
        application/commandHandlers/deleteChannelCommandHandler/DeleteChannelCommandHandlerImpl.cpp

        domain/entities/userChannel/UserChannel.cpp
        infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.cpp
        infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.cpp
        application/commandHandlers/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.cpp
        application/commandHandlers/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImpl.cpp
        application/queryHandlers/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImpl.cpp
        application/queryHandlers/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.cpp
        api/MessageHandlers/UpdateUserMessageHandler.cpp
        api/MessageHandlers/DeleteUserMessageHandler.cpp
        api/MessageHandlers/VerifyUserMessageHandle.cpp

        domain/entities/channelInvitation/ChannelInvitation.cpp
        infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.cpp
        infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.cpp
        application/commandHandlers/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImpl.cpp
        application/commandHandlers/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImpl.cpp

        api/MessageHandlers/UpdateUserMessageHandler.cpp
        api/MessageHandlers/DeleteUserMessageHandler.cpp
        api/MessageHandlers/LogoutMessageHandler.cpp)

set(UT_SOURCES
        api/SessionImplTest.cpp
        api/SessionManagerTest.cpp
        infrastructure/services/emailService/EmailServiceImplTest.cpp
        api/MessageRouterImplTest.cpp
        api/MessageHandlers/CreateChannelMessageHandlerTest.cpp
        api/MessageHandlers/DeleteUserMessageHandlerTest.cpp
        api/MessageHandlers/GetUserChannelsMessageHandlerTest.cpp
        api/MessageHandlers/GetUserDataMessageHandlerTest.cpp
        api/MessageHandlers/LoginMessageHandlerTest.cpp
        api/MessageHandlers/RegisterMessageHandlerTest.cpp
        api/MessageHandlers/UpdateUserMessageHandlerTest.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImplTest.cpp
        infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImplTest.cpp
        infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImplTest.cpp
        application/commandHandlers/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImplTest.cpp
        api/MessageHandlers/VerifyUserMessageHandleTest.cpp api/MessageHandlers/LogoutMessageHandlerTest.cpp)

set(IT_SOURCES
        infrastructure/repositories/userRepository/UserRepositoryImplIntegrationTest.cpp
        application/services/tokenService/TokenServiceImplIntegrationTest.cpp
        application/services/hashService/HashServiceImplIntegrationTest.cpp
        application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/deleteUserCommandHandler/DeleteUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/updateUserCommandHandler/UpdateUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/logoutUserCommandHandler/LogoutUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/findUserQueryHandler/FindUserQueryHandlerImplIntegrationTest.cpp
        infrastructure/repositories/channelRepository/ChannelRepositoryImplIntegrationTest.cpp
        application/commandHandlers/createChannelCommandHandler/CreateChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/deleteChannelCommandHandler/DeleteChannelCommandHandlerImplIntegrationTest.cpp
        infrastructure/repositories/userChannelRepository/UserChannelRepositoryImplIntegrationTest.cpp
        application/commandHandlers/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImplIntegrationTest.cpp
        infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImplIntegrationTest.cpp
        application/commandHandlers/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImplIntegrationTest.cpp
        )

set(ODB_HEADERS
        infrastructure/database/entities/User.h
        infrastructure/database/entities/Channel.h
        infrastructure/database/entities/UserChannel.h
        infrastructure/database/entities/ChannelInvitation.h
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
        infrastructure/database/entities/User.cpp
        infrastructure/database/entities/Channel.cpp
        infrastructure/database/entities/UserChannel.cpp
        infrastructure/database/entities/ChannelInvitation.cpp
        ${ODB_GENERATED_SOURCES}
        )

set(ODB_LIBRARIES
        odb
        odb-pgsql
        )

include_directories(infrastructure/database/entities)

add_executable(${EXECUTABLE_NAME}
        Main.cpp
        ${SOURCES}
        ${ODB_SOURCES}
        )

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
        Qt6::Core
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        httpClient
        ${ODB_LIBRARIES}
        )

target_include_directories(${EXECUTABLE_NAME} PRIVATE
        ${DOTENV_INCLUDE_DIR}
        )

add_executable(${EXECUTABLE_NAME}UT
        ${UT_SOURCES}
        ${SOURCES}
        ${ODB_SOURCES}
        )

target_link_libraries(${EXECUTABLE_NAME}UT PRIVATE
        GTest::gmock_main
        Qt6::Core
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        httpClient
        ${ODB_LIBRARIES}
        faker-cxx
        )

target_include_directories(${EXECUTABLE_NAME}UT PUBLIC ${GMOCK_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}UT COMMAND ${EXECUTABLE_NAME}UT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_code_coverage(${EXECUTABLE_NAME}UT ALL)

add_executable(${EXECUTABLE_NAME}IT
        ${IT_SOURCES}
        ${SOURCES}
        ${ODB_SOURCES}
        )

target_link_libraries(${EXECUTABLE_NAME}IT PRIVATE
        GTest::gtest_main
        Qt6::Core
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        httpClient
        ${ODB_LIBRARIES}
        faker-cxx
        )

target_include_directories(${EXECUTABLE_NAME}IT PUBLIC ${GMOCK_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}IT COMMAND ${EXECUTABLE_NAME}IT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_code_coverage(${EXECUTABLE_NAME}IT ALL)
