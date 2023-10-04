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
        infrastructure/services/emailService/EmailServiceImpl.cpp
        application/services/hashService/HashServiceImpl.cpp
        application/services/tokenService/TokenServiceImpl.cpp
        application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerImpl.cpp
        application/commandHandlers/user/deleteUserCommandHandler/DeleteUserCommandHandlerImpl.cpp
        application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandlerImpl.cpp
        application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandlerImpl.cpp
        application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerImpl.cpp
        application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImpl.cpp
        application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImpl.cpp
        application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerImpl.cpp
        application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandlerImpl.cpp

        domain/entities/channel/Channel.cpp
        domain/entities/userChannel/UserChannel.cpp
        domain/entities/channelInvitation/ChannelInvitation.cpp
        infrastructure/repositories/channelRepository/ChannelRepositoryImpl.cpp
        infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.cpp
        infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.cpp
        infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.cpp
        infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.cpp
        infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.cpp
        application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandlerImpl.cpp
        application/commandHandlers/channel/deleteChannelCommandHandler/DeleteChannelCommandHandlerImpl.cpp
        application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.cpp
        application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImpl.cpp
        application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImpl.cpp
        application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.cpp
        application/queryHandlers/channel/findChannelByIdQueryHandler/FindChannelByIdQueryHandlerImpl.cpp
        application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerImpl.cpp
        application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImpl.cpp
        application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImpl.cpp
        application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerImpl.cpp

        domain/entities/friendInvitation/FriendInvitation.cpp
        domain/entities/friendship/Friendship.cpp
        infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.cpp
        infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.cpp
        infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.cpp
        infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.cpp
        application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerImpl.cpp
        application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerImpl.cpp
        application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerImpl.cpp
        application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImpl.cpp
        application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerImpl.cpp
        application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerImpl.cpp
        application/queryHandlers/friend/findUserFriendsQueryHandler/FindUserFriendsQueryHandlerImpl.cpp

        domain/entities/conversation/Conversation.cpp
        infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapperImpl.cpp
        infrastructure/repositories/conversationRepository/ConversationRepositoryImpl.cpp

        api/MessageHandlers/UpdateUserMessageHandler.cpp
        api/MessageHandlers/DeleteUserMessageHandler.cpp
        api/MessageHandlers/VerifyUserMessageHandle.cpp
        api/MessageHandlers/LogoutMessageHandler.cpp
        api/MessageHandlers/SendChannelInvitationMessageHandler.cpp
        api/MessageHandlers/LeftTheChannelMessageHandler.cpp
        api/MessageHandlers/DeleteTheChannelMessageHandler.cpp
        api/MessageHandlers/FriendRequestMessageHandler.cpp
        )

set(TEST_COMMON_SOURCES
        tests/factories/userTestFactory/UserTestFactory.cpp
        tests/factories/channelTestFactory/ChannelTestFactory.cpp
        tests/factories/userChannelTestFactory/UserChannelTestFactory.cpp
        tests/factories/friendshipTestFactory/FriendshipTestFactory.cpp
        tests/factories/friendInvitationTestFactory/FriendInvitationTestFactory.cpp
        tests/factories/channelInvitationTestFactory/ChannelInvitationTestFactory.cpp
        tests/factories/conversationTestFactory/ConversationTestFactory.cpp
        )

set(UT_SOURCES
        api/SessionImplTest.cpp
        api/SessionManagerTest.cpp
        api/MessageRouterImplTest.cpp
        api/MessageHandlers/CreateChannelMessageHandlerTest.cpp
        api/MessageHandlers/DeleteUserMessageHandlerTest.cpp
        api/MessageHandlers/GetUserChannelsMessageHandlerTest.cpp
        api/MessageHandlers/GetUserDataMessageHandlerTest.cpp
        api/MessageHandlers/LoginMessageHandlerTest.cpp
        api/MessageHandlers/RegisterMessageHandlerTest.cpp
        api/MessageHandlers/UpdateUserMessageHandlerTest.cpp
        api/MessageHandlers/LogoutMessageHandlerTest.cpp
        api/MessageHandlers/VerifyUserMessageHandleTest.cpp
        infrastructure/services/emailService/EmailServiceImplTest.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImplTest.cpp
        infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImplTest.cpp
        infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImplTest.cpp
        application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImplTest.cpp
        infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImplTest.cpp
        infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImplTest.cpp
        infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImplTest.cpp
        infrastructure/repositories/conversationRepository/conversationMapper/ConversationMapperImplTest.cpp
        api/MessageHandlers/SendChannelInvitationMessageHandlerTest.cpp
        api/MessageHandlers/FriendRequestMessageHandlerTest.cpp api/MessageHandlers/DeleteTheChannelMessageHandlerTest.cpp api/MessageHandlers/LeftTheChannelMessageHandlerTest.cpp)

set(IT_SOURCES
        tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.cpp
        tests/utils/userTestUtils/UserTestUtils.cpp
        tests/utils/channelTestUtils/ChannelTestUtils.cpp
        tests/utils/userChannelTestUtils/UserChannelTestUtils.cpp
        tests/utils/channelInvitationTestUtils/ChannelInvitationTestUtils.cpp
        tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.cpp
        tests/utils/friendshipTestUtils/FriendshipTestUtils.cpp
        tests/utils/conversationTestUtils/ConversationTestUtils.cpp

        infrastructure/repositories/userRepository/UserRepositoryImplIntegrationTest.cpp
        application/services/tokenService/TokenServiceImplIntegrationTest.cpp
        application/services/hashService/HashServiceImplIntegrationTest.cpp
        application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/user/deleteUserCommandHandler/DeleteUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandlerImplIntegrationTest.cpp

        infrastructure/repositories/channelRepository/ChannelRepositoryImplIntegrationTest.cpp
        infrastructure/repositories/userChannelRepository/UserChannelRepositoryImplIntegrationTest.cpp
        infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImplIntegrationTest.cpp
        application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/deleteChannelCommandHandler/DeleteChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findChannelByIdQueryHandler/FindChannelByIdQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerImplIntegrationTest.cpp

        infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImplIntegrationTest.cpp
        infrastructure/repositories/friendshipRepository/FriendshipRepositoryImplIntegrationTest.cpp
        application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/friend/findUserFriendsQueryHandler/FindUserFriendsQueryHandlerImplIntegrationTest.cpp

        infrastructure/repositories/conversationRepository/ConversationRepositoryImplIntegrationTest.cpp
        )

set(ODB_HEADERS
        infrastructure/database/entities/User.h
        infrastructure/database/entities/Channel.h
        infrastructure/database/entities/UserChannel.h
        infrastructure/database/entities/ChannelInvitation.h
        infrastructure/database/entities/FriendInvitation.h
        infrastructure/database/entities/Friendship.h
        infrastructure/database/entities/Conversation.h
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
        infrastructure/database/entities/FriendInvitation.cpp
        infrastructure/database/entities/Friendship.cpp
        infrastructure/database/entities/Conversation.cpp
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
        ${TEST_COMMON_SOURCES}
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
        ${TEST_COMMON_SOURCES}
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
