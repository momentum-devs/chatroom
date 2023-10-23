#include "MessageRouterFactory.h"

#include "httpClient/HttpClientFactory.h"
#include "MessageHandlers/AcceptChannelInvitationMessageHandler.h"
#include "MessageHandlers/CreateChannelMessageHandler.h"
#include "MessageHandlers/DeleteTheChannelMessageHandler.h"
#include "MessageHandlers/DeleteUserMessageHandler.h"
#include "MessageHandlers/FriendRequestMessageHandler.h"
#include "MessageHandlers/GetUserChannelInvitationsMessageHandler.h"
#include "MessageHandlers/GetUserChannelsMessageHandler.h"
#include "MessageHandlers/GetUserDataMessageHandler.h"
#include "MessageHandlers/GetUserFriendRequestsMessageHandler.h"
#include "MessageHandlers/LeftTheChannelMessageHandler.h"
#include "MessageHandlers/LoginMessageHandler.h"
#include "MessageHandlers/LogoutMessageHandler.h"
#include "MessageHandlers/RegisterMessageHandler.h"
#include "MessageHandlers/RejectChannelInvitationMessageHandler.h"
#include "MessageHandlers/SendChannelInvitationMessageHandler.h"
#include "MessageHandlers/UpdateUserMessageHandler.h"
#include "MessageHandlers/VerifyUserMessageHandle.h"
#include "MessageRouterImpl.h"
#include "server/api/MessageHandlers/AcceptFriendRequestMessageHandler.h"
#include "server/api/MessageHandlers/GetUserFriendsMessageHandler.h"
#include "server/api/MessageHandlers/RejectFriendRequestMessageHandler.h"
#include "server/application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/deleteChannelCommandHandler/DeleteChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/createFriendshipCommandHandler/CreateFriendshipCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandlerImpl.h"
#include "server/application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/deleteUserCommandHandler/DeleteUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImpl.h"
#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.h"
#include "server/application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandlerImpl.h"
#include "server/application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandlerImpl.h"
#include "server/application/queryHandlers/friend/findUserFriendsQueryHandler/FindUserFriendsQueryHandlerImpl.h"
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
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/infrastructure/services/emailService/EmailServiceImpl.h"

namespace server::api
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

    auto addUserToChannelCommandHandler = std::make_shared<server::application::AddUserToChannelCommandHandlerImpl>(
        userChannelRepository, userRepository, channelRepository);

    auto createChannelCommandHandler = std::make_unique<server::application::CreateChannelCommandHandlerImpl>(
        channelRepository, addUserToChannelCommandHandler, userRepository);

    const std::shared_ptr<common::httpClient::HttpClient> httpClient =
        common::httpClient::HttpClientFactory::createHttpClient();

    const auto emailService =
        std::make_shared<application::EmailServiceImpl>(httpClient, sendGridApiKey, sendGridEmail);

    auto sendRegistrationVerificationEmailCommandHandler =
        std::make_shared<server::application::SendRegistrationVerificationEmailCommandHandlerImpl>(userRepository,
                                                                                                   emailService);

    auto createChannelMessageHandler =
        std::make_shared<CreateChannelMessageHandler>(tokenService, std::move(createChannelCommandHandler));

    auto getUserChannelsMessageHandler = std::make_shared<GetUserChannelsMessageHandler>(
        tokenService, std::move(findChannelsToWhichUserBelongsQueryHandler));

    auto loginMessageHandler = std::make_shared<LoginMessageHandler>(std::move(loginUserCommandHandler));

    auto registerMessageHandler = std::make_shared<RegisterMessageHandler>(
        std::move(registerUserCommandHandler), sendRegistrationVerificationEmailCommandHandler);

    auto findUserQueryHandler = std::make_shared<server::application::FindUserQueryHandlerImpl>(userRepository);

    auto getUserDataMessageHandler = std::make_shared<GetUserDataMessageHandler>(tokenService, findUserQueryHandler);

    auto updateUserCommandHandler =
        std::make_unique<server::application::UpdateUserCommandHandlerImpl>(userRepository, hashService);

    auto updateUserMessageHandler =
        std::make_shared<UpdateUserMessageHandler>(tokenService, std::move(updateUserCommandHandler));

    auto deleteUserCommandHandler = std::make_unique<server::application::DeleteUserCommandHandlerImpl>(userRepository);

    auto deleteUserMessageHandler =
        std::make_shared<DeleteUserMessageHandler>(tokenService, std::move(deleteUserCommandHandler));

    auto verifyUserEmailCommandHandler =
        std::make_unique<server::application::VerifyUserEmailCommandHandlerImpl>(userRepository);

    auto verifyUserMessageHandler = std::make_shared<VerifyUserMessageHandle>(
        tokenService, std::move(verifyUserEmailCommandHandler), findUserQueryHandler);

    auto logoutCommandHandler = std::make_unique<server::application::LogoutUserCommandHandlerImpl>(userRepository);

    auto logoutMessageHandler = std::make_shared<LogoutMessageHandler>(tokenService, std::move(logoutCommandHandler));

    auto findUserByEmailQueryHandler =
        std::make_shared<server::application::FindUserByEmailQueryHandlerImpl>(userRepository);

    auto createChannelInvitationCommandHandler =
        std::make_unique<server::application::CreateChannelInvitationCommandHandlerImpl>(
            channelInvitationRepository, userRepository, channelRepository, userChannelRepository);

    auto sendChannelInvitationMessageHandler = std::make_shared<SendChannelInvitationMessageHandler>(
        tokenService, findUserByEmailQueryHandler, std::move(createChannelInvitationCommandHandler));

    auto removeUserFromChannelCommandHandler =
        std::make_unique<server::application::RemoveUserFromChannelCommandHandlerImpl>(userChannelRepository,
                                                                                       channelRepository);

    auto leftTheChannelMessageHandler =
        std::make_shared<LeftTheChannelMessageHandler>(tokenService, std::move(removeUserFromChannelCommandHandler));

    auto deleteChannelCommandHandler =
        std::make_unique<server::application::DeleteChannelCommandHandlerImpl>(channelRepository);

    auto deleteTheChannelMessageHandler =
        std::make_shared<DeleteTheChannelMessageHandler>(tokenService, std::move(deleteChannelCommandHandler));

    auto friendInvitationMapper = std::make_shared<server::infrastructure::FriendInvitationMapperImpl>(userMapper);

    auto friendInvitationRepository = std::make_shared<server::infrastructure::FriendInvitationRepositoryImpl>(
        db, friendInvitationMapper, userMapper);

    auto friendshipMapper = std::make_shared<server::infrastructure::FriendshipMapperImpl>(userMapper);
    auto friendshipRepository =
        std::make_shared<server::infrastructure::FriendshipRepositoryImpl>(db, friendshipMapper, userMapper);

    auto createFriendInvitationCommandHandler =
        std::make_unique<server::application::CreateFriendInvitationCommandHandlerImpl>(
            friendInvitationRepository, userRepository, friendshipRepository);

    auto friendRequestMessageHandler = std::make_shared<FriendRequestMessageHandler>(
        tokenService, findUserByEmailQueryHandler, std::move(createFriendInvitationCommandHandler));

    auto findReceivedChannelInvitationsQueryHandler =
        std::make_unique<server::application::FindReceivedChannelInvitationsQueryHandlerImpl>(
            channelInvitationRepository);
    auto getUserChannelInvitationsMessageHandler = std::make_shared<GetUserChannelInvitationsMessageHandler>(
        tokenService, std::move(findReceivedChannelInvitationsQueryHandler));

    auto acceptChannelInvitationCommandHandler =
        std::make_unique<server::application::AcceptChannelInvitationCommandHandlerImpl>(
            channelInvitationRepository, userRepository, addUserToChannelCommandHandler);

    auto acceptChannelInvitationMessageHandler = std::make_shared<AcceptChannelInvitationMessageHandler>(
        tokenService, std::move(acceptChannelInvitationCommandHandler));

    auto rejectChannelInvitationCommandHandler =
        std::make_unique<server::application::RejectChannelInvitationCommandHandlerImpl>(channelInvitationRepository,
                                                                                         userRepository);
    auto rejectChannelInvitationMessageHandler = std::make_shared<RejectChannelInvitationMessageHandler>(
        tokenService, std::move(rejectChannelInvitationCommandHandler));

    auto findReceivedFriendInvitationsQueryHandler =
        std::make_unique<server::application::FindReceivedFriendInvitationsQueryHandlerImpl>(
            friendInvitationRepository);

    auto getUserFriendRequestsMessageHandler = std::make_shared<GetUserFriendRequestsMessageHandler>(
        tokenService, std::move(findReceivedFriendInvitationsQueryHandler));

    auto createFriendshipCommandHandler =
        std::make_shared<application::CreateFriendshipCommandHandlerImpl>(friendshipRepository, userRepository);

    auto acceptFriendInvitationCommandHandler =
        std::make_unique<server::application::AcceptFriendInvitationCommandHandlerImpl>(
            friendInvitationRepository, userRepository, createFriendshipCommandHandler);

    auto acceptFriendRequestMessageHandler = std::make_shared<AcceptFriendRequestMessageHandler>(
        tokenService, std::move(acceptFriendInvitationCommandHandler));

    auto rejectFriendInvitationCommandHandlerImpl =
        std::make_unique<server::application::RejectFriendInvitationCommandHandlerImpl>(friendInvitationRepository,
                                                                                        userRepository);

    auto rejectFriendRequestMessageHandler = std::make_shared<RejectFriendRequestMessageHandler>(
        tokenService, std::move(rejectFriendInvitationCommandHandlerImpl));

    auto findUserFriendsQueryHandler =
        std::make_unique<server::application::FindUserFriendsQueryHandlerImpl>(friendshipRepository);

    auto getUserFriendsMessageHandler =
        std::make_shared<GetUserFriendsMessageHandler>(tokenService, std::move(findUserFriendsQueryHandler));

    std::unordered_map<common::messages::MessageId, std::shared_ptr<MessageHandler>> messageHandlers{
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
        {common::messages::MessageId::SendFriendRequest, friendRequestMessageHandler},
        {common::messages::MessageId::GetUserChannelInvitations, getUserChannelInvitationsMessageHandler},
        {common::messages::MessageId::AcceptChannelInvitation, acceptChannelInvitationMessageHandler},
        {common::messages::MessageId::RejectChannelInvitation, rejectChannelInvitationMessageHandler},
        {common::messages::MessageId::GetFriendRequests, getUserFriendRequestsMessageHandler},
        {common::messages::MessageId::AcceptFriendRequests, acceptFriendRequestMessageHandler},
        {common::messages::MessageId::RejectFriendRequests, rejectFriendRequestMessageHandler},
        {common::messages::MessageId::GetUserFriends, getUserFriendsMessageHandler},
    };

    return std::make_unique<MessageRouterImpl>(std::move(messageHandlers));
}
}
