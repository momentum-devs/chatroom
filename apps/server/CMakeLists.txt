set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(ODB)

set(EXECUTABLE_NAME server)

set(SOURCES
        config/ConfigProvider.cpp
        core/session/SessionManager.cpp
        core/session/SessionImpl.cpp
        core/session/SessionFactoryImpl.cpp
        core/session/ConnectionAcceptorImpl.cpp
        core/session/MessageRouterImpl.cpp
        core/session/MessageRouterFactory.cpp
        core/database/DatabaseConnectionFactory.cpp
        core/database/DatabaseMigrationsRunner.cpp

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
        application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImpl.cpp
        application/commandHandlers/channel/leaveChannelCommandHandler/LeaveChannelCommandHandlerImpl.cpp
        application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImpl.cpp
        application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.cpp
        application/queryHandlers/channel/findChannelByIdQueryHandler/FindChannelByIdQueryHandlerImpl.cpp
        application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerImpl.cpp
        application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImpl.cpp
        application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImpl.cpp
        application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerImpl.cpp
        application/commandHandlers/channel/updateChannelCommandHandler/UpdateChannelCommandHandlerImpl.cpp

        domain/entities/friendInvitation/FriendInvitation.cpp
        domain/entities/friendship/Friendship.cpp
        infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.cpp
        infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.cpp
        infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.cpp
        infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.cpp
        application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerImpl.cpp
        application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerImpl.cpp
        application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerImpl.cpp
        application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerImpl.cpp
        application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerImpl.cpp
        application/queryHandlers/friend/findUserFriendshipsQueryHandler/FindUserFriendshipsQueryHandlerImpl.cpp

        domain/entities/group/Group.cpp
        domain/entities/userGroup/UserGroup.cpp
        infrastructure/repositories/groupRepository/GroupRepositoryImpl.cpp
        infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.cpp
        infrastructure/repositories/userGroupRepository/UserGroupRepositoryImpl.cpp
        infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.cpp
        application/commandHandlers/group/createGroupCommandHandler/CreateGroupCommandHandlerImpl.cpp
        application/commandHandlers/group/addUserToGroupCommandHandler/AddUserToGroupCommandHandlerImpl.cpp
        application/queryHandlers/group/findUsersBelongingToGroupQueryHandler/FindUsersBelongingToGroupQueryHandlerImpl.cpp
        application/queryHandlers/group/findGroupsToWhichUserBelongsQueryHandler/FindGroupsToWhichUserBelongsQueryHandlerImpl.cpp

        api/messageHandlers/channel/createChannelMessageHandler/CreateChannelMessageHandler.cpp
        api/messageHandlers/channel/getUserChannelsMessageHandler/GetUserChannelsMessageHandler.cpp
        api/messageHandlers/channel/sendChannelInvitationMessageHandler/SendChannelInvitationMessageHandler.cpp
        api/messageHandlers/channel/deleteChannelMessageHandler/DeleteChannelMessageHandler.cpp
        api/messageHandlers/channel/acceptChannelInvitationMessageHandler/AcceptChannelInvitationMessageHandler.cpp
        api/messageHandlers/channel/rejectChannelInvitationMessageHandler/RejectChannelInvitationMessageHandler.cpp
        api/messageHandlers/channel/leaveChannelMessageHandler/LeaveChannelMessageHandler.cpp
        api/messageHandlers/channel/getUserChannelInvitationsMessageHandler/GetUserChannelInvitationsMessageHandler.cpp
        api/messageHandlers/channel/getChannelMembersMessageHandler/GetChannelMembersMessageHandler.cpp
        api/messageHandlers/user/registerMessageHandler/RegisterMessageHandler.cpp
        api/messageHandlers/user/loginMessageHandler/LoginMessageHandler.cpp
        api/messageHandlers/user/getUserDataMessageHandler/GetUserDataMessageHandler.cpp
        api/messageHandlers/user/updateUserMessageHandler/UpdateUserMessageHandler.cpp
        api/messageHandlers/user/deleteUserMessageHandler/DeleteUserMessageHandler.cpp
        api/messageHandlers/user/verifyUserMessageHandler/VerifyUserMessageHandler.cpp
        api/messageHandlers/user/logoutMessageHandler/LogoutMessageHandler.cpp
        api/messageHandlers/friend/friendInvitationMessageHandler/FriendInvitationMessageHandler.cpp
        api/messageHandlers/friend/getUserFriendInvitationsMessageHandler/GetUserFriendInvitationsMessageHandler.cpp
        api/messageHandlers/friend/acceptFriendInvitationMessageHandler/AcceptFriendInvitationMessageHandler.cpp
        api/messageHandlers/friend/rejectFriendInvitationMessageHandler/RejectFriendInvitationMessageHandler.cpp
        api/messageHandlers/friend/getUserFriendsMessageHandler/GetUserFriendsMessageHandler.cpp
        api/messageHandlers/friend/removeFromFriendsMessageHandler/RemoveFromFriendsMessageHandler.cpp
        api/messageHandlers/messages/sendMessageToChannelMessageHandler/SendMessageToChannelMessageHandler.cpp
        api/messageHandlers/messages/getMessagesFromChannelMessageHandler/GetMessagesFromChannelMessageHandler.cpp

        domain/entities/message/Message.cpp
        infrastructure/repositories/messageRepository/MessageRepositoryImpl.cpp
        infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.cpp
        application/commandHandlers/message/createGroupMessageCommandHandler/CreateGroupMessageCommandHandlerImpl.cpp
        application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandlerImpl.cpp
        application/commandHandlers/message/deleteMessageCommandHandler/DeleteMessageCommandHandlerImpl.cpp
        application/commandHandlers/message/updateMessageCommandHandler/UpdateMessageCommandHandlerImpl.cpp
        application/commandHandlers/message/setLastChannelMessageReadByUserCommandHandler/SetLastChannelMessageReadByUserCommandHandlerImpl.cpp
        application/commandHandlers/message/setLastGroupMessageReadByUserCommandHandler/SetLastGroupMessageReadByUserCommandHandlerImpl.cpp
        application/queryHandlers/message/findChannelMessagesQueryHandler/FindChannelMessagesQueryHandlerImpl.cpp
        application/queryHandlers/message/findGroupMessagesQueryHandler/FindGroupMessagesQueryHandlerImpl.cpp

        domain/entities/reaction/Reaction.cpp
        infrastructure/repositories/reactionRepository/ReactionRepositoryImpl.cpp
        infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapperImpl.cpp
        application/commandHandlers/message/createMessageReactionCommandHandler/CreateMessageReactionCommandHandlerImpl.cpp
        application/commandHandlers/message/deleteMessageReactionCommandHandler/DeleteMessageReactionCommandHandlerImpl.cpp
        application/queryHandlers/message/findMessageReactionsQueryHandler/FindMessageReactionsQueryHandlerImpl.cpp

        domain/entities/blacklistToken/BlacklistToken.cpp
        infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapperImpl.cpp
        infrastructure/repositories/blacklistTokenRepository/BlacklistTokenRepositoryImpl.cpp
)

set(TEST_COMMON_SOURCES
        tests/factories/userTestFactory/UserTestFactory.cpp
        tests/factories/channelTestFactory/ChannelTestFactory.cpp
        tests/factories/userChannelTestFactory/UserChannelTestFactory.cpp
        tests/factories/friendshipTestFactory/FriendshipTestFactory.cpp
        tests/factories/friendInvitationTestFactory/FriendInvitationTestFactory.cpp
        tests/factories/channelInvitationTestFactory/ChannelInvitationTestFactory.cpp
        tests/factories/groupTestFactory/GroupTestFactory.cpp
        tests/factories/userGroupTestFactory/UserGroupTestFactory.cpp
        tests/factories/messageTestFactory/MessageTestFactory.cpp
        tests/factories/reactionTestFactory/ReactionTestFactory.cpp
        tests/factories/blacklistTokenTestFactory/BlacklistTokenTestFactory.cpp
)

set(UT_SOURCES
        TestUnitMain.cpp
        core/session/SessionImplTest.cpp
        core/session/SessionManagerTest.cpp
        core/session/MessageRouterImplTest.cpp

        infrastructure/services/emailService/EmailServiceImplTest.cpp
        infrastructure/repositories/userRepository/userMapper/UserMapperImplTest.cpp
        infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImplTest.cpp
        infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImplTest.cpp
        application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImplTest.cpp
        infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImplTest.cpp
        infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImplTest.cpp
        infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImplTest.cpp
        infrastructure/repositories/groupRepository/groupMapper/GroupMapperImplTest.cpp
        infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImplTest.cpp
        infrastructure/repositories/messageRepository/messageMapper/MessageMapperImplTest.cpp
        infrastructure/repositories/reactionRepository/reactionMapper/ReactionMapperImplTest.cpp
        infrastructure/repositories/blacklistTokenRepository/blacklistTokenMapper/BlacklistTokenMapperImplTest.cpp

        api/messageHandlers/channel/createChannelMessageHandler/CreateChannelMessageHandlerTest.cpp
        api/messageHandlers/channel/getUserChannelsMessageHandler/GetUserChannelsMessageHandlerTest.cpp
        api/messageHandlers/channel/sendChannelInvitationMessageHandler/SendChannelInvitationMessageHandlerTest.cpp
        api/messageHandlers/channel/deleteChannelMessageHandler/DeleteChannelMessageHandlerTest.cpp
        api/messageHandlers/channel/leaveChannelMessageHandler/LeaveChannelMessageHandlerTest.cpp
        api/messageHandlers/channel/getUserChannelInvitationsMessageHandler/GetUserChannelInvitationsMessageHandlerTest.cpp
        api/messageHandlers/channel/acceptChannelInvitationMessageHandler/AcceptChannelInvitationMessageHandlerTest.cpp
        api/messageHandlers/channel/rejectChannelInvitationMessageHandler/RejectChannelInvitationMessageHandlerTest.cpp
        api/messageHandlers/channel/getChannelMembersMessageHandler/GetChannelMembersMessageHandlerTest.cpp
        api/messageHandlers/user/deleteUserMessageHandler/DeleteUserMessageHandlerTest.cpp
        api/messageHandlers/user/getUserDataMessageHandler/GetUserDataMessageHandlerTest.cpp
        api/messageHandlers/user/loginMessageHandler/LoginMessageHandlerTest.cpp
        api/messageHandlers/user/registerMessageHandler/RegisterMessageHandlerTest.cpp
        api/messageHandlers/user/updateUserMessageHandler/UpdateUserMessageHandlerTest.cpp
        api/messageHandlers/user/logoutMessageHandler/LogoutMessageHandlerTest.cpp
        api/messageHandlers/user/verifyUserMessageHandler/VerifyUserMessageHandlerTest.cpp
        api/messageHandlers/friend/friendInvitationMessageHandler/FriendInvitationMessageHandlerTest.cpp
        api/messageHandlers/friend/getUserFriendInvitationsMessageHandler/GetUserFriendInvitationsMessageHandlerTest.cpp
        api/messageHandlers/friend/acceptFriendInvitationMessageHandler/AcceptFriendInvitationMessageHandlerTest.cpp
        api/messageHandlers/friend/rejectFriendInvitationMessageHandler/RejectFriendInvitationMessageHandlerTest.cpp
        api/messageHandlers/friend/getUserFriendsMessageHandler/GetUserFriendsMessageHandlerTest.cpp
        api/messageHandlers/friend/removeFromFriendsMessageHandler/RemoveFromFriendsMessageHandlerTest.cpp
        api/messageHandlers/messages/sendMessageToChannelMessageHandler/SendMessageToChannelMessageHandlerTest.cpp
        api/messageHandlers/messages/getMessagesFromChannelMessageHandler/GetMessagesFromChannelMessageHandlerTest.cpp
)

set(IT_SOURCES
        TestIntegrationMain.cpp
        tests/factories/databaseClientTestFactory/DatabaseClientTestFactory.cpp
        tests/utils/userTestUtils/UserTestUtils.cpp
        tests/utils/channelTestUtils/ChannelTestUtils.cpp
        tests/utils/userChannelTestUtils/UserChannelTestUtils.cpp
        tests/utils/channelInvitationTestUtils/ChannelInvitationTestUtils.cpp
        tests/utils/friendInvitationTestUtils/FriendInvitationTestUtils.cpp
        tests/utils/friendshipTestUtils/FriendshipTestUtils.cpp
        tests/utils/groupTestUtils/GroupTestUtils.cpp
        tests/utils/userGroupTestUtils/UserGroupTestUtils.cpp
        tests/utils/messageTestUtils/MessageTestUtils.cpp
        tests/utils/reactionTestUtils/ReactionTestUtils.cpp
        tests/utils/blacklistTokenTestUtils/BlacklistTokenTestUtils.cpp

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
        application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/leaveChannelCommandHandler/LeaveChannelCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findChannelByIdQueryHandler/FindChannelByIdQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/channel/updateChannelCommandHandler/UpdateChannelCommandHandlerImplIntegrationTest.cpp

        infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImplIntegrationTest.cpp
        infrastructure/repositories/friendshipRepository/FriendshipRepositoryImplIntegrationTest.cpp
        application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/friend/findUserFriendshipsQueryHandler/FindUserFriendshipsQueryHandlerImplIntegrationTest.cpp

        infrastructure/repositories/groupRepository/GroupRepositoryImplIntegrationTest.cpp
        application/commandHandlers/group/createGroupCommandHandler/CreateGroupCommandHandlerImplIntegrationTest.cpp
        infrastructure/repositories/userGroupRepository/UserGroupRepositoryImplIntegrationTest.cpp
        application/commandHandlers/group/addUserToGroupCommandHandler/AddUserToGroupCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/group/findUsersBelongingToGroupQueryHandler/FindUsersBelongingToGroupQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/group/findGroupsToWhichUserBelongsQueryHandler/FindGroupsToWhichUserBelongsQueryHandlerImplIntegrationTest.cpp

        infrastructure/repositories/messageRepository/MessageRepositoryImplIntegrationTest.cpp
        application/commandHandlers/message/createGroupMessageCommandHandler/CreateGroupMessageCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/message/deleteMessageCommandHandler/DeleteMessageCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/message/updateMessageCommandHandler/UpdateMessageCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/message/setLastChannelMessageReadByUserCommandHandler/SetLastChannelMessageReadByUserCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/message/setLastGroupMessageReadByUserCommandHandler/SetLastGroupMessageReadByUserCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/message/findChannelMessagesQueryHandler/FindChannelMessagesQueryHandlerImplIntegrationTest.cpp
        application/queryHandlers/message/findGroupMessagesQueryHandler/FindGroupMessagesQueryHandlerImplIntegrationTest.cpp

        infrastructure/repositories/reactionRepository/ReactionRepositoryImplIntegrationTest.cpp
        application/commandHandlers/message/createMessageReactionCommandHandler/CreateMessageReactionCommandHandlerImplIntegrationTest.cpp
        application/commandHandlers/message/deleteMessageReactionCommandHandler/DeleteMessageReactionCommandHandlerImplIntegrationTest.cpp
        application/queryHandlers/message/findMessageReactionsQueryHandler/FindMessageReactionsQueryHandlerImplIntegrationTest.cpp

        infrastructure/repositories/blacklistTokenRepository/BlacklistTokenRepositoryImplIntegrationTest.cpp
)

set(ODB_HEADERS
        infrastructure/database/entities/User.h
        infrastructure/database/entities/Channel.h
        infrastructure/database/entities/UserChannel.h
        infrastructure/database/entities/ChannelInvitation.h
        infrastructure/database/entities/FriendInvitation.h
        infrastructure/database/entities/Friendship.h
        infrastructure/database/entities/Group.h
        infrastructure/database/entities/UserGroup.h
        infrastructure/database/entities/Message.h
        infrastructure/database/entities/Reaction.h
        infrastructure/database/entities/BlacklistToken.h
)

odb_compile(
        HEADERS ${ODB_HEADERS}
        DB sqlite
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
        infrastructure/database/entities/Group.cpp
        infrastructure/database/entities/UserGroup.cpp
        infrastructure/database/entities/Message.cpp
        infrastructure/database/entities/Reaction.cpp
        infrastructure/database/entities/BlacklistToken.cpp
        ${ODB_GENERATED_SOURCES}
)

set(ODB_LIBRARIES
        odb
        odb-sqlite
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
        random
        ${ODB_LIBRARIES}
        envParser
        sqlite3
        fmt::fmt
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
        Qt6::Core
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        httpClient
        random
        ${ODB_LIBRARIES}
        faker-cxx
        envParser
        gtest
        gmock
        sqlite3
        fmt::fmt
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
        Qt6::Core
        loguru::loguru
        nlohmann_json::nlohmann_json
        cpp-jwt::cpp-jwt
        messages
        filesystem
        collection
        httpClient
        random
        ${ODB_LIBRARIES}
        faker-cxx
        envParser
        sqlite3
        gtest
        gmock
        fmt::fmt
)

target_include_directories(${EXECUTABLE_NAME}IT PUBLIC ${GMOCK_INCLUDE_DIR} ${DOTENV_INCLUDE_DIR})

add_test(NAME ${EXECUTABLE_NAME}IT COMMAND ${EXECUTABLE_NAME}IT WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_code_coverage(${EXECUTABLE_NAME}IT ALL)
