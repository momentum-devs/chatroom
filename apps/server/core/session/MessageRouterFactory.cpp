#include "MessageRouterFactory.h"

#include "httpClient/HttpClientFactory.h"
#include "MessageRouterImpl.h"
#include "server/api/messageHandlers/channel/acceptChannelInvitationMessageHandler/AcceptChannelInvitationMessageHandler.h"
#include "server/api/messageHandlers/channel/createChannelMessageHandler/CreateChannelMessageHandler.h"
#include "server/api/messageHandlers/channel/deleteChannelMessageHandler/DeleteChannelMessageHandler.h"
#include "server/api/messageHandlers/channel/getChannelMembersMessageHandler/GetChannelMembersMessageHandler.h"
#include "server/api/messageHandlers/channel/getUserChannelInvitationsMessageHandler/GetUserChannelInvitationsMessageHandler.h"
#include "server/api/messageHandlers/channel/getUserChannelsMessageHandler/GetUserChannelsMessageHandler.h"
#include "server/api/messageHandlers/channel/leaveChannelMessageHandler/LeaveChannelMessageHandler.h"
#include "server/api/messageHandlers/channel/rejectChannelInvitationMessageHandler/RejectChannelInvitationMessageHandler.h"
#include "server/api/messageHandlers/channel/sendChannelInvitationMessageHandler/SendChannelInvitationMessageHandler.h"
#include "server/api/messageHandlers/friend/acceptFriendInvitationMessageHandler/AcceptFriendInvitationMessageHandler.h"
#include "server/api/messageHandlers/friend/friendInvitationMessageHandler/FriendInvitationMessageHandler.h"
#include "server/api/messageHandlers/friend/getUserFriendInvitationsMessageHandler/GetUserFriendInvitationsMessageHandler.h"
#include "server/api/messageHandlers/friend/getUserFriendsMessageHandler/GetUserFriendsMessageHandler.h"
#include "server/api/messageHandlers/friend/rejectFriendInvitationMessageHandler/RejectFriendInvitationMessageHandler.h"
#include "server/api/messageHandlers/friend/removeFromFriendsMessageHandler/RemoveFromFriendsMessageHandler.h"
#include "server/api/messageHandlers/messages/getChannelMessagesMessageHandler/GetChannelMessagesMessageHandler.h"
#include "server/api/messageHandlers/messages/sendChannelMessage/SendChannelMessageHandler.h"
#include "server/api/messageHandlers/user/deleteUserMessageHandler/DeleteUserMessageHandler.h"
#include "server/api/messageHandlers/user/getUserDataMessageHandler/GetUserDataMessageHandler.h"
#include "server/api/messageHandlers/user/loginMessageHandler/LoginMessageHandler.h"
#include "server/api/messageHandlers/user/logoutMessageHandler/LogoutMessageHandler.h"
#include "server/api/messageHandlers/user/registerMessageHandler/RegisterMessageHandler.h"
#include "server/api/messageHandlers/user/updateUserMessageHandler/UpdateUserMessageHandler.h"
#include "server/api/messageHandlers/user/verifyUserMessageHandler/VerifyUserMessageHandler.h"
#include "server/application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/deleteChannelCommandHandler/DeleteChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/leaveChannelCommandHandler/LeaveChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/deleteUserCommandHandler/DeleteUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImpl.h"
#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.h"
#include "server/application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerImpl.h"
#include "server/application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandlerImpl.h"
#include "server/application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerImpl.h"
#include "server/application/queryHandlers/friend/findUserFriendsQueryHandler/FindUserFriendsQueryHandlerImpl.h"
#include "server/application/queryHandlers/message/findChannelMessagesQueryHandler/FindChannelMessagesQueryHandlerImpl.h"
#include "server/application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandlerImpl.h"
#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/channelInvitationMapper/ChannelInvitationMapperImpl.h"
#include "server/infrastructure/repositories/channelInvitationRepository/ChannelInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/friendInvitationMapper/FriendInvitationMapperImpl.h"
#include "server/infrastructure/repositories/friendInvitationRepository/FriendInvitationRepositoryImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/friendshipMapper/FriendshipMapperImpl.h"
#include "server/infrastructure/repositories/friendshipRepository/FriendshipRepositoryImpl.h"
#include "server/infrastructure/repositories/groupRepository/groupMapper/GroupMapperImpl.h"
#include "server/infrastructure/repositories/groupRepository/GroupRepositoryImpl.h"
#include "server/infrastructure/repositories/messageRepository/messageMapper/MessageMapperImpl.h"
#include "server/infrastructure/repositories/messageRepository/MessageRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/userGroupMapper/UserGroupMapperImpl.h"
#include "server/infrastructure/repositories/userGroupRepository/UserGroupRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/infrastructure/services/emailService/EmailServiceImpl.h"

namespace server::core
{
MessageRouterFactory::MessageRouterFactory(std::shared_ptr<odb::pgsql::database> dbInit, const std::string& jwtSecret,
                                           const int jwtExpireIn, std::string sendGridApiKeyInit,
                                           std::string sendGridEmailInit)
    : db{std::move(dbInit)},
      hashService{std::make_shared<server::application::HashServiceImpl>()},
      tokenService{std::make_shared<server::application::TokenServiceImpl>(jwtSecret, jwtExpireIn)},
      sendGridApiKey{std::move(sendGridApiKeyInit)},
      sendGridEmail{std::move(sendGridEmailInit)}
{
}

std::unique_ptr<MessageRouter> MessageRouterFactory::createMessageRouter() const
{
    auto userMapper = std::make_shared<server::infrastructure::UserMapperImpl>();

    auto userRepository = std::make_shared<server::infrastructure::UserRepositoryImpl>(db, userMapper);

    auto groupMapper = std::make_shared<server::infrastructure::GroupMapperImpl>();

    auto groupRepository = std::make_shared<server::infrastructure::GroupRepositoryImpl>(db, groupMapper);

    auto userGroupMapper = std::make_shared<server::infrastructure::UserGroupMapperImpl>(userMapper, groupMapper);

    auto userGroupRepository =
        std::make_shared<server::infrastructure::UserGroupRepositoryImpl>(db, userGroupMapper, userMapper, groupMapper);

    auto registerUserCommandHandler =
        std::make_unique<server::application::RegisterUserCommandHandlerImpl>(userRepository, hashService);

    auto loginUserCommandHandler =
        std::make_unique<server::application::LoginUserCommandHandlerImpl>(userRepository, hashService, tokenService);

    auto channelMapper = std::make_shared<server::infrastructure::ChannelMapperImpl>(userMapper);

    auto channelRepository =
        std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, channelMapper, userMapper);

    auto userChannelMapper = std::make_shared<server::infrastructure::UserChannelMapperImpl>(userMapper, channelMapper);

    auto userChannelRepository = std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(
        db, userChannelMapper, userMapper, channelMapper);

    auto channelInvitationMapper =
        std::make_shared<server::infrastructure::ChannelInvitationMapperImpl>(userMapper, channelMapper);

    auto channelInvitationRepository = std::make_shared<server::infrastructure::ChannelInvitationRepositoryImpl>(
        db, channelInvitationMapper, userMapper, channelMapper);

    auto findChannelsToWhichUserBelongsQueryHandler =
        std::make_unique<server::application::FindChannelsToWhichUserBelongsQueryHandlerImpl>(userChannelRepository);

    auto createChannelCommandHandler = std::make_unique<server::application::CreateChannelCommandHandlerImpl>(
        channelRepository, userRepository, userChannelRepository);

    const std::shared_ptr<common::httpClient::HttpClient> httpClient =
        common::httpClient::HttpClientFactory::createHttpClient();

    const auto emailService =
        std::make_shared<application::EmailServiceImpl>(httpClient, sendGridApiKey, sendGridEmail);

    auto sendRegistrationVerificationEmailCommandHandler =
        std::make_shared<server::application::SendRegistrationVerificationEmailCommandHandlerImpl>(userRepository,
                                                                                                   emailService);

    auto createChannelMessageHandler =
        std::make_shared<api::CreateChannelMessageHandler>(tokenService, std::move(createChannelCommandHandler));

    auto getUserChannelsMessageHandler = std::make_shared<api::GetUserChannelsMessageHandler>(
        tokenService, std::move(findChannelsToWhichUserBelongsQueryHandler));

    auto loginMessageHandler = std::make_shared<api::LoginMessageHandler>(std::move(loginUserCommandHandler));

    auto registerMessageHandler = std::make_shared<api::RegisterMessageHandler>(
        std::move(registerUserCommandHandler), sendRegistrationVerificationEmailCommandHandler);

    auto findUserQueryHandler = std::make_shared<server::application::FindUserQueryHandlerImpl>(userRepository);

    auto getUserDataMessageHandler =
        std::make_shared<api::GetUserDataMessageHandler>(tokenService, findUserQueryHandler);

    auto updateUserCommandHandler =
        std::make_unique<server::application::UpdateUserCommandHandlerImpl>(userRepository, hashService);

    auto updateUserMessageHandler =
        std::make_shared<api::UpdateUserMessageHandler>(tokenService, std::move(updateUserCommandHandler));

    auto deleteUserCommandHandler = std::make_unique<server::application::DeleteUserCommandHandlerImpl>(
        userRepository, userGroupRepository, groupRepository);

    auto deleteUserMessageHandler =
        std::make_shared<api::DeleteUserMessageHandler>(tokenService, std::move(deleteUserCommandHandler));

    auto verifyUserEmailCommandHandler =
        std::make_unique<server::application::VerifyUserEmailCommandHandlerImpl>(userRepository);

    auto verifyUserMessageHandler = std::make_shared<api::VerifyUserMessageHandler>(
        tokenService, std::move(verifyUserEmailCommandHandler), findUserQueryHandler);

    auto logoutCommandHandler = std::make_unique<server::application::LogoutUserCommandHandlerImpl>(userRepository);

    auto logoutMessageHandler =
        std::make_shared<api::LogoutMessageHandler>(tokenService, std::move(logoutCommandHandler));

    auto findUserByEmailQueryHandler =
        std::make_shared<server::application::FindUserByEmailQueryHandlerImpl>(userRepository);

    auto createChannelInvitationCommandHandler =
        std::make_unique<server::application::CreateChannelInvitationCommandHandlerImpl>(
            channelInvitationRepository, userRepository, channelRepository, userChannelRepository);

    auto sendChannelInvitationMessageHandler = std::make_shared<api::SendChannelInvitationMessageHandler>(
        tokenService, findUserByEmailQueryHandler, std::move(createChannelInvitationCommandHandler));

    auto leaveChannelCommandHandler =
        std::make_unique<server::application::LeaveChannelCommandHandlerImpl>(userChannelRepository, channelRepository);

    auto leftTheChannelMessageHandler =
        std::make_shared<api::LeaveChannelMessageHandler>(tokenService, std::move(leaveChannelCommandHandler));

    auto deleteChannelCommandHandler =
        std::make_unique<server::application::DeleteChannelCommandHandlerImpl>(channelRepository);

    auto deleteTheChannelMessageHandler =
        std::make_shared<api::DeleteChannelMessageHandler>(tokenService, std::move(deleteChannelCommandHandler));

    auto friendInvitationMapper = std::make_shared<server::infrastructure::FriendInvitationMapperImpl>(userMapper);

    auto friendInvitationRepository = std::make_shared<server::infrastructure::FriendInvitationRepositoryImpl>(
        db, friendInvitationMapper, userMapper);

    auto friendshipMapper = std::make_shared<server::infrastructure::FriendshipMapperImpl>(userMapper);
    auto friendshipRepository =
        std::make_shared<server::infrastructure::FriendshipRepositoryImpl>(db, friendshipMapper, userMapper);

    auto createFriendInvitationCommandHandler =
        std::make_unique<server::application::CreateFriendInvitationCommandHandlerImpl>(
            friendInvitationRepository, userRepository, friendshipRepository);

    auto friendInvitationMessageHandler = std::make_shared<api::FriendInvitationMessageHandler>(
        tokenService, findUserByEmailQueryHandler, std::move(createFriendInvitationCommandHandler));

    auto findReceivedChannelInvitationsQueryHandler =
        std::make_unique<server::application::FindReceivedChannelInvitationsQueryHandlerImpl>(
            channelInvitationRepository);
    auto getUserChannelInvitationsMessageHandler = std::make_shared<api::GetUserChannelInvitationsMessageHandler>(
        tokenService, std::move(findReceivedChannelInvitationsQueryHandler));

    auto acceptChannelInvitationCommandHandler =
        std::make_unique<server::application::AcceptChannelInvitationCommandHandlerImpl>(
            channelInvitationRepository, userRepository, userChannelRepository, channelRepository);

    auto acceptChannelInvitationMessageHandler = std::make_shared<api::AcceptChannelInvitationMessageHandler>(
        tokenService, std::move(acceptChannelInvitationCommandHandler));

    auto rejectChannelInvitationCommandHandler =
        std::make_unique<server::application::RejectChannelInvitationCommandHandlerImpl>(channelInvitationRepository,
                                                                                         userRepository);
    auto rejectChannelInvitationMessageHandler = std::make_shared<api::RejectChannelInvitationMessageHandler>(
        tokenService, std::move(rejectChannelInvitationCommandHandler));

    auto findReceivedFriendInvitationsQueryHandler =
        std::make_unique<server::application::FindReceivedFriendInvitationsQueryHandlerImpl>(
            friendInvitationRepository);

    auto getUserFriendInvitationsMessageHandler = std::make_shared<api::GetUserFriendInvitationsMessageHandler>(
        tokenService, std::move(findReceivedFriendInvitationsQueryHandler));

    auto acceptFriendInvitationCommandHandler =
        std::make_unique<server::application::AcceptFriendInvitationCommandHandlerImpl>(
            friendInvitationRepository, userRepository, friendshipRepository);

    auto acceptFriendInvitationMessageHandler = std::make_shared<api::AcceptFriendInvitationMessageHandler>(
        tokenService, std::move(acceptFriendInvitationCommandHandler));

    auto rejectFriendInvitationCommandHandlerImpl =
        std::make_unique<server::application::RejectFriendInvitationCommandHandlerImpl>(friendInvitationRepository,
                                                                                        userRepository);

    auto rejectFriendInvitationMessageHandler = std::make_shared<api::RejectFriendInvitationMessageHandler>(
        tokenService, std::move(rejectFriendInvitationCommandHandlerImpl));

    auto findUserFriendsQueryHandler =
        std::make_unique<server::application::FindUserFriendsQueryHandlerImpl>(friendshipRepository);

    auto getUserFriendsMessageHandler =
        std::make_shared<api::GetUserFriendsMessageHandler>(tokenService, std::move(findUserFriendsQueryHandler));

    auto deleteFriendshipCommandHandler =
        std::make_unique<server::application::DeleteFriendshipCommandHandlerImpl>(friendshipRepository, userRepository);

    auto removeFromFriendsMessageHandler =
        std::make_shared<api::RemoveFromFriendsMessageHandler>(tokenService, std::move(deleteFriendshipCommandHandler));

    auto findUsersBelongingToChannelQueryHandler =
        std::make_unique<application::FindUsersBelongingToChannelQueryHandlerImpl>(userChannelRepository);

    auto getChannelMembersMessageHandler = std::make_shared<api::GetChannelMembersMessageHandler>(
        tokenService, std::move(findUsersBelongingToChannelQueryHandler));

    auto messageMapper =
        std::make_shared<server::infrastructure::MessageMapperImpl>(userMapper, channelMapper, groupMapper);

    auto messageRepository = std::make_shared<server::infrastructure::MessageRepositoryImpl>(
        db, messageMapper, userMapper, channelMapper, groupMapper);

    auto createChannelMessageCommandHandler =
        std::make_unique<server::application::CreateChannelMessageCommandHandlerImpl>(
            channelRepository, userRepository, userChannelRepository, messageRepository);

    auto sendChannelMessageHandler = std::make_shared<server::api::SendChannelMessageHandler>(
        tokenService, std::move(createChannelMessageCommandHandler));

    auto findChannelMessagesQueryHandler =
        std::make_unique<server::application::FindChannelMessagesQueryHandlerImpl>(messageRepository);

    auto getChannelMessagesMessageHandler = std::make_shared<server::api::GetChannelMessagesMessageHandler>(
        tokenService, std::move(findChannelMessagesQueryHandler));

    std::unordered_map<common::messages::MessageId, std::shared_ptr<api::MessageHandler>> messageHandlers{
        {common::messages::MessageId::CreateChannel, createChannelMessageHandler},
        {common::messages::MessageId::GetUserChannels, getUserChannelsMessageHandler},
        {common::messages::MessageId::Login, loginMessageHandler},
        {common::messages::MessageId::Register, registerMessageHandler},
        {common::messages::MessageId::GetUserData, getUserDataMessageHandler},
        {common::messages::MessageId::UpdateUser, updateUserMessageHandler},
        {common::messages::MessageId::DeleteUser, deleteUserMessageHandler},
        {common::messages::MessageId::VerifyUser, verifyUserMessageHandler},
        {common::messages::MessageId::Logout, logoutMessageHandler},
        {common::messages::MessageId::SendChannelInvitation, sendChannelInvitationMessageHandler},
        {common::messages::MessageId::LeftTheChannel, leftTheChannelMessageHandler},
        {common::messages::MessageId::DeleteTheChannel, deleteTheChannelMessageHandler},
        {common::messages::MessageId::SendFriendInvitation, friendInvitationMessageHandler},
        {common::messages::MessageId::GetUserChannelInvitations, getUserChannelInvitationsMessageHandler},
        {common::messages::MessageId::AcceptChannelInvitation, acceptChannelInvitationMessageHandler},
        {common::messages::MessageId::RejectChannelInvitation, rejectChannelInvitationMessageHandler},
        {common::messages::MessageId::GetFriendInvitations, getUserFriendInvitationsMessageHandler},
        {common::messages::MessageId::AcceptFriendInvitations, acceptFriendInvitationMessageHandler},
        {common::messages::MessageId::RejectFriendInvitations, rejectFriendInvitationMessageHandler},
        {common::messages::MessageId::GetUserFriends, getUserFriendsMessageHandler},
        {common::messages::MessageId::RemoveFromFriends, removeFromFriendsMessageHandler},
        {common::messages::MessageId::GetChannelMembers, getChannelMembersMessageHandler},
        {common::messages::MessageId::SendChannelMessage, sendChannelMessageHandler},
        {common::messages::MessageId::GetChannelMessages, getChannelMessagesMessageHandler},
    };

    return std::make_unique<MessageRouterImpl>(std::move(messageHandlers));
}
}
