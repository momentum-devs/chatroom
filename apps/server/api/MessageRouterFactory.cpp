#include "MessageRouterFactory.h"

#include "httpClient/HttpClientFactory.h"
#include "MessageHandlers/CreateChannelMessageHandler.h"
#include "MessageHandlers/DeleteUserMessageHandler.h"
#include "MessageHandlers/GetUserChannelsMessageHandler.h"
#include "MessageHandlers/GetUserDataMessageHandler.h"
#include "MessageHandlers/LoginMessageHandler.h"
#include "MessageHandlers/RegisterMessageHandler.h"
#include "MessageHandlers/UpdateUserMessageHandler.h"
#include "MessageHandlers/VerifyUserMessageHandle.h"
#include "MessageRouterImpl.h"
#include "server/api/MessageHandlers/LogoutMessageHandler.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/deleteUserCommandHandler/DeleteUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandlerImpl.h"
#include "server/application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandlerImpl.h"
#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandlerImpl.h"
#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandlerImpl.h"
#include "server/application/services/hashService/HashServiceImpl.h"
#include "server/application/services/tokenService/TokenServiceImpl.h"
#include "server/infrastructure/repositories/channelRepository/channelMapper/ChannelMapperImpl.h"
#include "server/infrastructure/repositories/channelRepository/ChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/userChannelMapper/UserChannelMapperImpl.h"
#include "server/infrastructure/repositories/userChannelRepository/UserChannelRepositoryImpl.h"
#include "server/infrastructure/repositories/userRepository/userMapper/UserMapperImpl.h"
#include "server/infrastructure/repositories/userRepository/UserRepositoryImpl.h"
#include "server/infrastructure/services/emailService/EmailServiceImpl.h"

namespace server::api
{
MessageRouterFactory::MessageRouterFactory(std::shared_ptr<odb::pgsql::database> dbInit, const std::string& jwtSecret,
                                           const int jwtExpireIn, std::string sendGridApiKeyInit)
    : db{std::move(dbInit)},
      hashService{std::make_shared<server::application::HashServiceImpl>()},
      tokenService{std::make_shared<server::application::TokenServiceImpl>(jwtSecret, jwtExpireIn)},
      sendGridApiKey{std::move(sendGridApiKeyInit)}
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

    auto channelMapper = std::make_shared<server::infrastructure::ChannelMapperImpl>();

    auto channelRepository = std::make_shared<server::infrastructure::ChannelRepositoryImpl>(db, channelMapper);

    auto userChannelMapper = std::make_shared<server::infrastructure::UserChannelMapperImpl>(userMapper, channelMapper);

    auto userChannelRepository = std::make_shared<server::infrastructure::UserChannelRepositoryImpl>(
        db, userChannelMapper, userMapper, channelMapper);

    auto findChannelsToWhichUserBelongsQueryHandler =
        std::make_unique<server::application::FindChannelsToWhichUserBelongsQueryHandlerImpl>(userChannelRepository);

    auto addUserToChannelCommandHandler = std::make_shared<server::application::AddUserToChannelCommandHandlerImpl>(
        userChannelRepository, userRepository, channelRepository);

    auto createChannelCommandHandler = std::make_unique<server::application::CreateChannelCommandHandlerImpl>(
        channelRepository, addUserToChannelCommandHandler);

    const std::shared_ptr<common::httpClient::HttpClient> httpClient =
        common::httpClient::HttpClientFactory::createHttpClient();

    const auto emailService = std::make_shared<application::EmailServiceImpl>(httpClient, sendGridApiKey);

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

    std::unordered_map<common::messages::MessageId, std::shared_ptr<MessageHandler>> messageHandlers{
        {common::messages::MessageId::CreateChannel, createChannelMessageHandler},
        {common::messages::MessageId::GetUserChannels, getUserChannelsMessageHandler},
        {common::messages::MessageId::Login, loginMessageHandler},
        {common::messages::MessageId::Register, registerMessageHandler},
        {common::messages::MessageId::GetUserData, getUserDataMessageHandler},
        {common::messages::MessageId::UpdateUser, updateUserMessageHandler},
        {common::messages::MessageId::DeleteUser, deleteUserMessageHandler},
        {common::messages::MessageId::VerifyUser, verifyUserMessageHandler},
        {common::messages::MessageId::Logout, logoutMessageHandler}};

    return std::make_unique<MessageRouterImpl>(std::move(messageHandlers));
}
}
